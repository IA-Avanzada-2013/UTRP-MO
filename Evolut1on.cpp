#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"
#include "Evolut1on.h"

#include <math.h>
#include <algorithm>

	Evolut1on::Evolut1on(Problem p, int seed, std::vector<RouteInfo> routes_info){
		this->p = p;
        this->seed=seed;
        srand ( this->seed );
        //routes_info row=id, col1=quantity, col2=min, col3=max
        this->routes_info = new int[routes_info.size()][3];
        for (int i = 0; i < routes_info.size(); ++i)
	    {
			this->routes_info[i][0]=routes_info[i].quantity;
			this->routes_info[i][1]=routes_info[i].min_length; 
			this->routes_info[i][2]=routes_info[i].max_length;
	    }
        //debug1
        std::cout << "Seed:" << std::endl;
        std::cout << this->seed << std::endl;

        std::cout << "Problem:" << std::endl;
        this->p.show_bus_stops();
		std::cout << "..." << std::endl;
		this->p.show_demand();
		std::cout << "..." << std::endl;
		this->p.show_travel_times();	

		std::cout << "RouteInfo:" << std::endl;
		std::cout << "\t" << this->route_type <<  ":";
		std::cout << this->total_routes << ":" << this->route_min_length << ":" << this->route_max_length << std::endl;


        //generate feasible routes sets
        //exec the algorithm


	};

	bool Evolut1on::find_node(std::vector<BusStop>actual, int idx){
		bool result=false;
		for(int i = 0; i< actual.size(); i++){
			if(actual[i].idi==idx)
				result=true;
		}
		return result;
	}

	std::vector<BusStop>  Evolut1on::get_distinct_neighbour_bus_stops(int previous_node,std::vector<BusStop> route_in_creation,std::vector<BusStop> bus_stops_left){
		std::vector<BusStop> out;
		int flag = 0;
		for (int i = 0; i < bus_stops_left.size();i++){
			flag = 0;
			if(previous_node == bus_stops_left[i].idi || !this->is_neighbour(bus_stops_left[i],previous_node) || this->find_node(route_in_creation,bus_stops_left[i].idi)){
				flag = 1;
			}
			if(flag == 0){
				out.push_back(bus_stops_left[i]);
			}
		}
	}

	bool Evolut1on::is_neighbour(BusStop a, int b){
		return(this->p.get_travel_times()[a.idi][b]>0);
	}

	std::vector<Route> Evolut1on::generate_feasible_route_set(){
		std::vector<BusStop> bus_stops = this->p.get_bus_stops();
		std::vector<Route> route_set;
		int number_of_routes = ceil(log(bus_stops.size())); //seemed appropiate for small-size instances
		int margin = ceil(number_of_routes/10);
		int min_nodes_per_route = ceil((bus_stops.size()/number_of_routes)-margin);
		int max_nodes_per_route = ceil((bus_stops.size()/number_of_routes)+margin);
		int tmp_route_size,tmp_route_start,tmp_route_end,previous_node;
		bool reversed;
		Route *tmp_route,*previous_route;
		std::vector<BusStop> tmp_bus_stop_route;
		std::vector<BusStop> left_bus_stops(bus_stops); //clones the bus stop vector
		std::vector<BusStop> neighbours;
		while(route_set.size()<number_of_routes+1){
			tmp_route = new Route();
			tmp_route_size = rand() % (margin*2) + min_nodes_per_route;
			while (left_bus_stops.size()>0){
				//while there are bus stops left, create route
				reversed=false;
				while(tmp_bus_stop_route.size()<tmp_route_size){
					//if its the first node of the route
					if(tmp_bus_stop_route.size()==0){
						//if its the first route
						if(route_set.size()==0){
							tmp_route_start = rand() % left_bus_stops.size();
							previous_node=tmp_route_start;
							tmp_bus_stop_route.push_back(left_bus_stops[tmp_route_start]);
						}
						else{
							//random node from previous route
							tmp_route_start = previous_route->bus_stops[rand() % previous_route->bus_stops.size()].idi;
							previous_node=tmp_route_start;
							tmp_bus_stop_route.push_back(left_bus_stops[tmp_route_start]);
						}
					}
					//if there are no bus stops left					
					if(left_bus_stops.size()==0){
						tmp_route_size=tmp_bus_stop_route.size();
					}
					else{
						neighbours = this->get_distinct_neighbour_bus_stops(previous_node,tmp_bus_stop_route,left_bus_stops);
						if (neighbours.size() != 0){
							tmp_bus_stop_route.push_back(neighbours[rand() % neighbours.size()]);
						}
						else{
							neighbours = this->get_distinct_neighbour_bus_stops(previous_node,tmp_bus_stop_route,bus_stops);
							if (neighbours.size() != 0){
								tmp_bus_stop_route.push_back(neighbours[rand() % neighbours.size()]);
							}
							else{
								if(reversed){
									//route has been reversed and still cant grow
									tmp_bus_stop_route.clear();
								}
								reverse(tmp_bus_stop_route.begin(),tmp_bus_stop_route.end());
								previous_node=tmp_bus_stop_route[0].idi;
								reversed = true;
							}
						}

					}
				}
				if (tmp_route->bus_stops.size()==tmp_route_size){
					route_set.push_back(tmp_route[0]);
					previous_route = tmp_route;
					//remove selected bus stops from left_bus_stops
					for( int j = 0; j < tmp_route->bus_stops.size(); j++){
						for (int k = 0; k < left_bus_stops.size(); ++k)
						{
							if(left_bus_stops[k].idi==tmp_route->bus_stops[j].idi)
								left_bus_stops.erase(left_bus_stops.begin()+k);
						}
					}
				}

			}
			return route_set;
		}
/*
1: {n = number of nodes; r = number of routes, MAX, MIN = maximum, minimum number of routes in route set, ADJ(i) = adjacency list for node i in transit network}
2: Initialize set Chosen ← ∅ {Nodes used so far in least one route}
3: for count ← 1 to r do
4:Determine the length of current route l at random from ∈{M IN, ..M AX}
5:if count = 1 then
6:Choose a node, i, at random ∈ {1, .., n} {Seed the first route}
7:Initialize route, route(1)(1) ← i
8:else
9:Choose a node, i, at random ∈ Chosen {Seed the next route,ensuring connectivity}
10:Chosen ← Chosen ∪ {i}
11:Initialize route, route(count)(1) ← i
12:length ← 1
13:while {Current route length less than l} AND {route has not been reversed more than once} do
14:U nused ← ADJ(i) \ route(count){Unused contains nodes adjacent to i, but currently absent from route(count)}
15:if U nused = ∅ then
16:length ← length + 1
17:Select a node, j adjacent to i ∈ U nused at random
18:route(count)(length) ← j {Add it to the end of the currentroute}
19:i ← j
20:Chosen ← Chosen ∪ {j}
21:else
22:Reverse the route, so that i ← route(1) {Finally, if not all nodes are yet present in the routeset, add them here}
23: if |Chosen| < n then
24:Call Repair{routeset, Chosen, n, r, M AX, M IN }
25:if routeset successfully repaired then
26:return routeset feasible
27:else
28:return routeset infeasible
*/

	}
	void Evolut1on::make_small_change(){

	}
	bool Evolut1on::check_duplicates(){

	}