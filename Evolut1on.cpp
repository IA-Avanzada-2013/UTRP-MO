#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"
#include "Evolut1on.h"
#include <algorithm>

Evolut1on::Evolut1on(Problem p, int seed, std::vector<RouteInfo> routes_info){
	this->p = p;
    this->seed=seed;
    srand ( this->seed );
    //routes_info row=id, col1=quantity, col2=min, col3=max
	this->routes_info = new int*[routes_info.size()];
	for (int i = 0; i < routes_info.size(); ++i){
		this->routes_info[i] = new int[3];
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
	this->route_types = routes_info.size();

	std::cout << "RouteInfo:" << std::endl;
	for (int i = 0; i < this->route_types; ++i)
	{
		std::cout << "Ruta " << i <<  ":";
		std::cout << this->routes_info[i][0] << ":" << this->routes_info[i][1] << ":" << this->routes_info[i][2]<< std::endl;
	}

    //generate feasible routes sets
    Solution sol = generate_feasible_route_set(routes_info)[0];
    std::cout << "Solution Generated:" << std::endl;
    

    this->result.solutions.push_back(sol);
	this->result.solutions[0].print_solution_routes();

    //exec the algorithm
};

Evolut1on::~Evolut1on(void){
}
//returns true if BusStop ID belongs to first route
bool Evolut1on::find_node(std::vector<BusStop>actual, int idx){
	bool result=false;
	for(int i = 0; i< actual.size(); i++){
		if(actual[i].idi==idx)
			result=true;
	}
	return result;
}

//returns a neighbour bus stop that is in the final vector but isnt in the actual created route
std::vector<BusStop>  Evolut1on::get_neighbour_bus_stops(int previous_node,std::vector<BusStop> route_in_creation,std::vector<BusStop> bus_stops_left){
	//std::cout << "Total BusStop Left Size: " <<  bus_stops_left.size() << std::endl;
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
	//std::cout << "neighbeours found for: " << previous_node;
	//this->print_temp_route(out);
	return out;
}

//returns true if nodes are neighbeours
bool Evolut1on::is_neighbour(BusStop a, int b){
	bool result = false;
	if(this->p.get_travel_times()[a.idi][b]>0)
		result=true;
	return result;
}

//selects a node which is actually in a route, neighbour to an unused node
int Evolut1on::nice_neighbour(std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops){
	int nice_neighbour =-1;
	int i = rand() % used_nodes.size();
	for (int j = 0; j < left_bus_stops.size(); ++j){
		//std::cout << "comparando vecinidad " << used_nodes[i].idi <<"," << left_bus_stops[j].idi<< std::endl;
		if(this->is_neighbour(used_nodes[i],left_bus_stops[j].idi)){
			nice_neighbour =  used_nodes[i].idi;
			//std::cout<<"Nice neighbour found: "<< nice_neighbour<<std::endl;
			return nice_neighbour;
		}
	}
		
	return nice_neighbour;
}

Route *Evolut1on::generate_route(int tmp_route_size,std::vector<Route> routes_so_far, int tmp_route_type){
	//std::cin.get();
	Solution actual_routeset;
	actual_routeset.set_routes(routes_so_far);
	std::vector<BusStop> used_nodes = this->get_specific_nodes(&actual_routeset,true);
	std::vector<BusStop> left_bus_stops= this->get_specific_nodes(&actual_routeset,false);

	//std::cout << "Nodes Used So Far: " << used_nodes.size();
	//this->print_temp_route(used_nodes);
	
	//std::cout << "Free Nodes Left: " << left_bus_stops.size();	
	//this->print_temp_route(left_bus_stops);


	//std::cout << "Route Generator Started"<< std::endl;
	//Actual route that is going to get returned
	Route *tmp_route = new Route();
	//BusStop vector that contains what will be the returned route
	std::vector<BusStop> tmp_bus_stop_route;
	//array giving neighbour bus stops to the current_node
	std::vector<BusStop> neighbours;
	//bool indicated if end of possible nodes was reached
	bool reversed;
	//BusStop which indicates the first node
	int tmp_route_start,previous_node, tmp_neighbour;
	reversed=false;
	while(tmp_bus_stop_route.size()<tmp_route_size){
		//if its the first node of the route
		if(tmp_bus_stop_route.size()==0){
			//std::cout << "Adding First Node"<< std::endl;
			//if its the first route
			if(used_nodes.size()==0){
				tmp_route_start = rand() % left_bus_stops.size();
				previous_node=tmp_route_start;
				tmp_bus_stop_route.push_back(left_bus_stops[tmp_route_start]);
			}
			else{
				//random already used node (with cool addon)
				tmp_route_start = this->nice_neighbour(used_nodes,left_bus_stops);
				if(tmp_route_start==-1)
					tmp_route_start = used_nodes[rand() % used_nodes.size()].idi;
				previous_node=tmp_route_start;
				tmp_bus_stop_route.push_back(left_bus_stops[tmp_route_start]);
			}
		}

		//std::cout << "Finding neighbours"<< std::endl;
		neighbours = this->get_neighbour_bus_stops(previous_node,tmp_bus_stop_route,left_bus_stops);
		if (neighbours.size() != 0){
			tmp_neighbour = rand() % neighbours.size();
			previous_node = neighbours[tmp_neighbour].idi;
			tmp_bus_stop_route.push_back(neighbours[tmp_neighbour]);
			//std::cout << "Adding Found neighbour: "<< previous_node<<std::endl;
		}
		else{
			//std::cout << "No neighbours found"<< std::endl;
			neighbours = this->get_neighbour_bus_stops(previous_node,tmp_bus_stop_route,this->p.get_bus_stops());
			if (neighbours.size() != 0){
				tmp_neighbour = rand() % neighbours.size();
				previous_node = neighbours[tmp_neighbour].idi;
				tmp_bus_stop_route.push_back(neighbours[tmp_neighbour]);
				//std::cout << "Adding Found neighbour in full set: "<< previous_node<<std::endl;
			}
			else{
				//std::cout << "No neighbours found in full set"<< std::endl;
				if(reversed){
					//route has been reversed and still cant grow

					//std::cout << "Route will be cleared: " << tmp_bus_stop_route.size();	
					this->print_temp_route(tmp_bus_stop_route);

					tmp_bus_stop_route.clear();
					reversed = false;
				}
				else{

					//std::cout << "Route will be reversed: " << tmp_bus_stop_route.size();	
					//this->print_temp_route(tmp_bus_stop_route);
					previous_node=tmp_bus_stop_route[0].idi;
					reverse(tmp_bus_stop_route.begin(),tmp_bus_stop_route.end());
					//std::cout << "Route SHOULD be reversed: " << tmp_bus_stop_route.size();	
					//this->print_temp_route(tmp_bus_stop_route);
					reversed = true;
				}
			}
		}

		
	}
	//add the bus stops to new route
	tmp_route->bus_stops = tmp_bus_stop_route;
	tmp_route->tipo_ruta = tmp_route_type;
	//std::cout << "Route (type,size): " << tmp_route->tipo_ruta<<","<<tmp_route->bus_stops.size()<< std::endl;
	//remove selected bus stops from left_bus_stops
	for( int j = 0; j < tmp_route->bus_stops.size(); j++){
		for (int k = 0; k < left_bus_stops.size(); ++k)
		{
			if(left_bus_stops[k].idi==tmp_route->bus_stops[j].idi){
				//std::cout << "node " << left_bus_stops[k].idi << " removed from left_bus_stops" <<std::endl;
				left_bus_stops.erase(left_bus_stops.begin()+k);
			}
		}
	}
	//std::cout << "New Route: ";
	//tmp_route->print_route();

	return tmp_route;
}

void Evolut1on::print_temp_route(std::vector<BusStop> ruta){
	if(ruta.size()==0)
		std::cout << " ( )"<< std::endl;
	else{
			std::cout << " (";
		for (int i = 0; i < ruta.size()-1; ++i)
			std::cout << ruta[i].idi << ",";
		std::cout << ruta[ruta.size()-1].idi<< ")"<< std::endl;
	}
}

Solution *Evolut1on::generate_feasible_route_set(std::vector<RouteInfo> routes_info){
	Solution *sol = new Solution();
	std::vector<BusStop> used_nodes;
	std::vector<Route> route_set;

	//calculate total routes needed
	int number_of_routes=0,routes_so_far=0;
	for (int i = 0; i < this->route_types; ++i)
		number_of_routes += this->routes_info[i][0];
	
	int diff_size = 0;
	bool used=false;
	int tmp_route_size, tmp_route_type;
	
	Route *tmp_route;

	for (int i = 0; i < number_of_routes; ++i){
		//std::cout << "Generating route nº:" << i << std::endl;
		//get the route type assigned to de i'th route
		routes_so_far=0;
		for (int j = 0; j < this->route_types; ++j)
		{
			routes_so_far+=this->routes_info[j][0];
			if (i<routes_so_far){
				diff_size = this->routes_info[j][2]-this->routes_info[j][1];
				tmp_route_size = rand() % diff_size + this->routes_info[j][1];
				tmp_route_type = j;
				break;
			}
		}
		tmp_route=this->generate_route(tmp_route_size,route_set,tmp_route_type);
		//check if route is not a duplicate

		route_set.push_back(tmp_route[0]);

		//add new used nodes to vector used_nodes
		for (int k = 0; k < route_set[i].bus_stops.size(); ++k){
			used=false;
			for (int l = 0; l < used_nodes.size(); ++l)
				if(used_nodes[l]==route_set[i].bus_stops[k])
					used=true;
			if(!used)
				used_nodes.push_back(route_set[i].bus_stops[k]);	
		}
	}
	sol->set_routes(route_set);
	//check feasibility
    if(sol->check_feasability(&routes_info, this->p.get_size())){
    	sol->setF01;
    	std::cout << "Its Feasible:" << std::endl;
    }
    else{
    	std::cout << "Its NOT Feasible:" << std::endl;

    	if(this->repair_routeset(sol)){
    		std::cout << "Route Repaired!" << std::endl;
    		if(sol->check_feasability(&routes_info, this->p.get_size()))	
				std::cout << "Now Its Feasible =)" << std::endl;
    	}
    	else
    		std::cout << "Repair Failed =(" << std::endl;
    }
	ShortestRoute *sr = new ShortestRoute(this->p.get_size());
    sr->calcDistNoRoutes(this->p.get_travel_times());
    sol->setFO1(sr,this->p.get_demand2());
    sol->setF02(this->p.get_size(),this->p.get_travel_times());

	return sol;
}
//statos = true if they are used nodes, false if not
std::vector<BusStop> Evolut1on::get_specific_nodes(Solution *routeset,bool status){
	std::vector<BusStop> result,tmp_bus_stops;
	bool flag;
	if(status){
		result = this->p.get_bus_stops();
		tmp_bus_stops = this->p.get_bus_stops();
		for (int i = 0; i < tmp_bus_stops.size(); ++i)
		{
			flag=false;
			for (int j = 0; j < routeset->routes.size(); ++j)
				for (int k = 0; k < routeset->routes[j].bus_stops.size(); ++k)
					if(tmp_bus_stops[i].idi==routeset->routes[j].bus_stops[k].idi)
						flag=true;
			if(!flag){
				flag=false;
				for (int j = 0; j < result.size(); ++j)
					if(result[j].idi==tmp_bus_stops[i].idi)
						result.erase(result.begin()+j);
			}
		}				
	}
	else{
		result = this->p.get_bus_stops();
		for (int i = 0; i < routeset->routes.size(); ++i)
			for (int j = 0; j < routeset->routes[i].bus_stops.size(); ++j)
				for (int k = 0; k < result.size(); ++k)
					if(result[k].idi==routeset->routes[i].bus_stops[j].idi)
						result.erase(result.begin()+k);
	}
	return result;
}

bool Evolut1on::repair_routeset(Solution *unfeasible_routeset){
	std::vector<BusStop> used_nodes = this->get_specific_nodes(unfeasible_routeset,true);
	std::vector<BusStop> left_bus_stops= this->get_specific_nodes(unfeasible_routeset,false);
	bool result=false;
	int attempts_timeout=0;
	for (int i = 0; i < this->route_types; ++i)
		attempts_timeout += this->routes_info[i][0];

	bool possibilities_exhausted = false;
	Route chosen_route;
	while((used_nodes.size()<this->p.get_size()) && !possibilities_exhausted){
		std::cout << "Starting repair attempt" << std::endl;
		chosen_route=unfeasible_routeset->routes[rand() % unfeasible_routeset->routes.size()];
		chosen_route.print_route();
		if(chosen_route.bus_stops.size()<this->routes_info[chosen_route.tipo_ruta][2]){
			std::cout << "Route can accept extra nodes" << std::endl;
			for (int i = 0; i < left_bus_stops.size(); ++i)	{
				std::cout << "There is/are " << left_bus_stops.size() << " bus stop(s) left" <<std::endl; 
				std::cout << "Checking left node "<< left_bus_stops[i].idi << std::endl;
				if(this->is_neighbour(chosen_route.bus_stops[0],left_bus_stops[i].idi) ){
					std::cout << "Node is Neighbour of the beginning" << std::endl;
					chosen_route.bus_stops.insert(chosen_route.bus_stops.begin(), left_bus_stops[i]);
					used_nodes.push_back(left_bus_stops[i]);
					left_bus_stops.erase(left_bus_stops.begin()+i);
					break;
				}
				else if(this->is_neighbour(chosen_route.bus_stops[chosen_route.bus_stops.size()-1],left_bus_stops[i].idi) ){
					std::cout << "Node is neighbour of end" << std::endl;
					chosen_route.bus_stops.push_back(left_bus_stops[i]);
					used_nodes.push_back(left_bus_stops[i]);
					left_bus_stops.erase(left_bus_stops.begin()+i);
					break;
				}
				else{
					std::cout << "Node "  << left_bus_stops[i].idi << " is not neighbour of route" << std::endl;
					attempts_timeout--;
					std::cout << "timeout: " << attempts_timeout << std::endl;
					if(attempts_timeout<0)
						possibilities_exhausted=true;
				}
			}
		}
	}

	if(!possibilities_exhausted)
		result=true;
	return result;
}

void Evolut1on::make_small_change(){

}
bool Evolut1on::check_duplicate_routes(Solution *routeset, Route *route){
	bool result;
	for (int i = 0; i < routeset->routes.size(); ++i){
		if(routeset->routes[i].bus_stops.size() == route->bus_stops.size()){
			result=true;
			for (int j = 0; j < routeset->routes[i].bus_stops.size(); ++j){
				if(routeset->routes[i].bus_stops[j].idi != route->bus_stops[j].idi)
					result=false;
			}
		}	
	}
	return result;
}


/*
Algorithm 1 Simple Multi-Objective Optimization (SMO)
Generate initial population of feasible route sets.
Calculate passenger and operator costs for each route set.
Record the best-route-set-so-far for both objectives.
repeat
for each route set in the population
Apply the Make-Small-Change procedure to produce a
feasible offspring
if offspring is a duplicate
then delete offspring
elseif offspring improves on either best-so-far
then offspring replaces parent and best-so-far updated
elseif offspring dominates parent
then offspring replaces parent
elseif offspring and parent are mutually non-dominated
then find an individual in the population that is dominated
by the offspring and replace it with the offspring.
endif
endfor
until the stopping condition is satisfied
print all non-dominated solutions in the final population.

*/