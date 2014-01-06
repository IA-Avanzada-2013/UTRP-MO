#include "Evolut1on.h"
#include <math.h>
#include <algorithm>

	Evolut1on::Evolut1on(Problem p){
		this->p = p;
	};

	bool Evolut1on::find_node(std::vector<BusStop>actual, int idx){
		bool result=false;
		for(int i = 0; i< actual.size(); i++){
			if(actual[i].idi==idx)
				result=true;
		}
		return result;
	}

	void Evolut1on::get_distinct_neighbour_bus_stops(int previous_node,std::vector<BusStop> actual,std::vector<BusStop> left, std::vector<BusStop> *out){
		int flag = 0;
		for (int i = 0; i < left.size();i++){
			flag = 0;
			if(previous_node == left[i].idi || !this->is_neighbour(left[i],previous_node) || this->find_node(actual,left[i].idi)){
				flag = 1;
			}
			if(flag == 0){
				(*out).push_back(left[i]);
			}
		}
	}

	bool Evolut1on::is_neighbour(BusStop a, int b){
		return(this->p.get_travel_times()[a.idi][b]>0);
	}

	void Evolut1on::generate_feasible_route_set(){
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
		std::vector<BusStop> tmp_bus_stops(bus_stops); //clones de bus stop vector
		std::vector<BusStop> neighbours;
		while(route_set.size()<number_of_routes+1){
			tmp_route = new Route();
			tmp_route_size = rand() % (margin*2) + min_nodes_per_route;
			while (tmp_bus_stops.size()>0){
				//while there are bus stops left, create route
				reversed=false;
				while(tmp_bus_stop_route.size()<tmp_route_size){
					//if its the first node of the route
					if(tmp_bus_stop_route.size()==0){
						//if its the first route
						if(route_set.size()==0){
							tmp_route_start = rand() % tmp_bus_stops.size();
							previous_node=tmp_route_start;
							tmp_bus_stop_route.push_back(tmp_bus_stops[tmp_route_start]);
						}
						else{
							//random node from previous route
							tmp_route_start = previous_route->bus_stops[rand() % previous_route->bus_stops.size()].idi;
							previous_node=tmp_route_start;
							tmp_bus_stop_route.push_back(tmp_bus_stops[tmp_route_start]);
						}
					}
					//if there are no bus stops left					
					if(tmp_bus_stops.size()==0){
						tmp_route_size=tmp_bus_stop_route.size();
					}
					else{
						this->get_distinct_neighbour_bus_stops(previous_node,tmp_bus_stop_route,tmp_bus_stops,&neighbours);
						if (neighbours.size() != 0){
							tmp_bus_stop_route.push_back(neighbours[rand() % neighbours.size()]);
						}
						else{
							this->get_distinct_neighbour_bus_stops(previous_node,tmp_bus_stop_route,bus_stops,&neighbours);
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
					//remove selected bus stops from tmp_bus_stops
					for( int j = 0; j < tmp_route->bus_stops.size(); j++){
						for (int k = 0; k < tmp_bus_stops.size(); ++k)
						{
							if(tmp_bus_stops[k].idi==tmp_route->bus_stops[j].idi)
								tmp_bus_stops.erase(tmp_bus_stops.begin()+k);
						}
					}
				}

			}
			
		}

/*
Parameters: number of routes, minimum and maximum number of
nodes for routes
Begin
Main loop
repeat
	Route length selection:
	Choose a length for the route between minimum and maximum
	number of nodes at random
	Inner Loop
	repeat
		Start node selection:
			If this is the first iteration of the loop, then randomly choose
				any node as the start node,
			Else Randomly choose a node from the previous route as a start
				node
			label this node as the “previous node”
		Next node selection:
			Construct a node set consisting of all nodes directly connected
			to “previous node” that have not been selected so far for any
			route.
		If this node set is not empty, choose a node from this set at
		random
		Else construct a node set consisting of all nodes directly con-
		nected to “previous node” that have not been selected for the
		current route.
		If this node set is not empty, choose a node from this set at
		random
			


										Else invert the order of the route and repeat the Next node
										selection
										If such route can not reach the route length, then delete this
										route and repeat Start node selection
	until route length is reached
until number of routes is reached
Output an initialization route set

++++

Input the route set, S
Input N, the number of nodes in the transit network
Initialize found-node[1...N] = 0 {records nodes that have been
found}
Initialize explored-node[1...N] = 0 {records nodes that have been
explored}
Select an arbitrary node, i, present in at least one route
Set feasibility = False
while {feasibility == False} AND {there are unexplored nodes in
found-node} do
Set explored-node[i] = found-node[i] = 1
Find all routes containing node i
Set flags in found-node to record all the nodes found in those
routes
Select any node from found-node that is absent from explored-
node
That node becomes node i
if all N nodes have been found and entered in found-node then
feasibility = True
return feasibility


*/

	}
	void Evolut1on::make_small_change(){

	}
	bool Evolut1on::check_duplicates(){

	}