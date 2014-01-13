#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"
#include "Evolut1on.h"
#include <algorithm>

#define SOLSET_SIZE 10 //nº of solutions in set

Evolut1on::Evolut1on(Problem p, int seed, std::vector<RouteInfo> routes_info){
	this->p = p;
    this->seed=seed;
    srand ( this->seed );
    this->route_types = routes_info.size();
    //routes_info row=id, col1=quantity, col2=min, col3=max
	this->routes_info = new int*[routes_info.size()];
	for (int i = 0; i < routes_info.size(); ++i){
		this->routes_info[i] = new int[3];
		this->routes_info[i][0]=routes_info[i].quantity;
		this->routes_info[i][1]=routes_info[i].min_length; 
		this->routes_info[i][2]=routes_info[i].max_length;
	}
    //debug1
    /*std::cout << "Seed:" << std::endl;
    std::cout << this->seed << std::endl;

    std::cout << "Problem:" << std::endl;
    this->p.show_bus_stops();
	std::cout << "..." << std::endl;
	this->p.show_demand();
	std::cout << "..." << std::endl;
	this->p.show_travel_times();
	

	std::cout << "RouteInfo:" << std::endl;
	for (int i = 0; i < this->route_types; ++i)
	{
		std::cout << "Ruta " << i <<  ":";
		std::cout << this->routes_info[i][0] << ":" << this->routes_info[i][1] << ":" << this->routes_info[i][2]<< std::endl;
	}
	*/
    //generate feasible routes sets
    for (int i = 0; i < SOLSET_SIZE; ++i)
    {
    	this->result.solutions.push_back(generate_feasible_route_set(routes_info)[0]);
    }
    std::cout << SOLSET_SIZE<< " Solutions Generated!" << std::endl;
    this->set_best_routes();
    this->result.print_solution_set();
    //exec the algorithm

    this->seamo2();
};

Evolut1on::~Evolut1on(void){
}

void Evolut1on::set_best_routes(){
	this->best_routeset_so_far_o1=0;
	this->best_routeset_so_far_o2=0;
	int tmp_o1=10000000,tmpo2=10000000;
	for (int i = 0; i < this->result.solutions.size(); ++i){
		if(result.solutions[i].fo1 < tmp_o1){
			this->best_routeset_so_far_o1 = i;
			tmp_o1 = result.solutions[i].fo1;
		}
		if(result.solutions[i].fo2 < tmpo2){
			this->best_routeset_so_far_o2 = i;
			tmpo2 = result.solutions[i].fo2;
		}
	}
}

void Evolut1on::seamo2(){

	/*
Fig. 3. SEAMO2 AlGORITHM
1: Generate initial population of feasible route sets
2: Calculate passenger and operator costs for each route set
3: Record the best-routeset-so-far for each objective
4: repeat
5:		for each individual in the population do
6:			This individual is Parent1
7:			Select a second individual at random (Parent2)
8:			Offspring ← Crossover(Parent1,Parent2)
9:			Repair(Offspring)
10:			Apply mutation(Offspring)
11:			if Offspring is a duplicate then
12:				Delete offspring
13:			else if Offspring dominates either parent then
14:				Offspring replaces the dominated parent, testing Parent 1 first,then Parent 2
15:			else if Offspring dominates either vector containing the best-so-far objective or Offspring improves on a best-so-far objective then
16:				Offspring replaces a parent, ensuring the other best-so-far objective is not lost
17:			else if Offspring and parent(s) are mutually non-dominating then
18:				Find an individual in the population that is dominated by the Offspring and replace it with the Offspring
19:			else
20:				Delete Offspring
21: until the stopping condition is satisfied
22: print All non-dominated solutions in the final population.
*/
	Solution parent1,parent2,*offspring;
	int rand_id;
	bool condition_satisfied=false;
	//while(!condition_satisfied){
		for (int i = 0; i < this->result.solutions.size(); ++i)
		{
			parent1 = this->result.solutions[i];
			do {rand_id = rand() % this->result.solutions.size();}while(rand_id!=i);
			parent2 = this->result.solutions[rand_id];
			offspring = this->crossover(parent1,parent2);
			this->make_small_change(offspring);//apply mutation
			if(this->check_feasability2(offspring)){
				std::cout<<"Yay1!"<<std::endl;
			}
			else{
				if(this->repair_routeset(offspring)){
					std::cout<<"Yay!"<<std::endl;
					if(this->check_feasability2(offspring))
						std::cout<<"Offspring is feasible now!"<<std::endl;
				}
				
				else
					std::cout<<"Boo!"<<std::endl;
			}
		}	
	//}
}

Solution *Evolut1on::crossover(Solution P1, Solution P2){
	Solution *result = new Solution();
	std::vector<BusStop> used_nodes,left_bus_stops;
	std::vector<Route> route_set;
		//calculate total routes needed
	int number_of_routes=0,tmp_nice_route;
	for (int i = 0; i < this->route_types; ++i)
		number_of_routes += this->routes_info[i][0];
	

	//first route
	route_set.push_back(P1.routes[rand() % P1.routes.size()]);
	for (int i = 1; i < number_of_routes; ++i){
		result->set_routes(route_set);
		used_nodes = this->get_specific_nodes(result,true);
		left_bus_stops= this->get_specific_nodes(result,false);
		if(i%2){//even node
			tmp_nice_route= this->nice_route(P1.routes,used_nodes,left_bus_stops);
			if(tmp_nice_route==-1)
				tmp_nice_route=rand()%P1.routes.size();
			//std::cout << "nice route = " << tmp_nice_route <<std::endl;
			route_set.push_back(P1.routes[tmp_nice_route]);
		}
		else{//odd node
			tmp_nice_route= this->nice_route(P2.routes,used_nodes,left_bus_stops);
			if(tmp_nice_route==-1)
				tmp_nice_route=rand()%P2.routes.size();
			//std::cout << "nice route = " << tmp_nice_route <<std::endl;
			route_set.push_back(P2.routes[tmp_nice_route]);
		}
	}
	result->set_routes(route_set);
	if(this->check_feasability2(result)){
    	this->set_fs(result);
    }
    else{
    	std::cout << "Its NOT Feasible:" << std::endl;

    	if(this->repair_routeset(result)){
    		std::cout << "Route Repaired!" << std::endl;
    		if(this->check_feasability2(result)){
    			std::cout << "Now Its Feasible =)" << std::endl;
				this->set_fs(result);
    		}
    	}
    	else
    		std::cout << "Repair Failed =(" << std::endl;
    }
	return result;
}

int Evolut1on::nice_route(std::vector<Route> route_set,std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops){
	int result=-1,tmp_count;
	float best_so_far=0;
	//returns the id of route which contains more unused nodes 
	for (int i = 0; i < route_set.size(); ++i){
		tmp_count=0;
		for (int j = 0; j < route_set[i].bus_stops.size(); ++j){
			if(this->find_node(left_bus_stops,route_set[i].bus_stops[j].idi)){
				//std::cout << "node found " <<std::endl;
				tmp_count++;
			}
		}
		//std::cout << "nice route "<< i << ": " << (float)tmp_count/route_set[i].bus_stops.size() <<std::endl;
		if(((float)tmp_count/route_set[i].bus_stops.size()<1) && ((float)tmp_count/route_set[i].bus_stops.size()>best_so_far)){

			best_so_far = (float)tmp_count/route_set[i].bus_stops.size();
			result=i;
		}
	}
	return result;
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
					//this->print_temp_route(tmp_bus_stop_route);

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
    	//std::cout << "Its Feasible:" << std::endl;
    	this->set_fs(sol);
    }
    else{
    	std::cout << "Its NOT Feasible:" << std::endl;

    	if(this->repair_routeset(sol)){
    		std::cout << "Route Repaired!" << std::endl;
    		if(sol->check_feasability(&routes_info, this->p.get_size())){
    			std::cout << "Now Its Feasible =)" << std::endl;
				this->set_fs(sol);
    		}	
				
    	}
    	else
    		std::cout << "Repair Failed =(" << std::endl;
    }
    

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

void Evolut1on::set_fs(Solution *sol){
    	int size = this->p.get_size();
		ShortestRoute *sr = new ShortestRoute(size);
		int ** travel_times = this->p.get_travel_times();
	    int ** demand = this->p.get_demand2();
	    sr->calcDist(travel_times,sol->routes);
	    sol->setF01(sr,demand);
	    sol->setF02(size,travel_times);
	    delete sr;
}

bool Evolut1on::repair_routeset(Solution *unfeasible_routeset){
	std::cin.get();
	std::vector<BusStop> used_nodes = this->get_specific_nodes(unfeasible_routeset,true);
	std::vector<BusStop> left_bus_stops= this->get_specific_nodes(unfeasible_routeset,false);
	bool result=false;
	int attempts_timeout=0;
	for (int i = 0; i < this->route_types; ++i)
		attempts_timeout += this->routes_info[i][0];
	attempts_timeout = attempts_timeout*left_bus_stops.size();

	bool possibilities_exhausted = false;
	Route chosen_route;
	while((used_nodes.size()<this->p.get_size()) && !possibilities_exhausted){
		std::cout << "Free Nodes Left: " << left_bus_stops.size();	
		this->print_temp_route(left_bus_stops);
		// std::cout << "Starting repair attempt" << std::endl;
		chosen_route=unfeasible_routeset->routes[rand() % unfeasible_routeset->routes.size()];
		chosen_route.print_route();
		if(chosen_route.bus_stops.size()<this->routes_info[chosen_route.tipo_ruta][2]){
			//std::cout << "Route can accept extra nodes" << std::endl;
			for (int i = 0; i < left_bus_stops.size(); ++i)	{
				//std::cout << "There is/are " << left_bus_stops.size() << " bus stop(s) left" <<std::endl; 
				//std::cout << "Checking left node "<< left_bus_stops[i].idi << std::endl;
				if(this->is_neighbour(chosen_route.bus_stops[0],left_bus_stops[i].idi) ){
					//std::cout << "Node is Neighbour of the beginning" << std::endl;
					chosen_route.bus_stops.insert(chosen_route.bus_stops.begin(), left_bus_stops[i]);
					used_nodes.push_back(left_bus_stops[i]);
					left_bus_stops.erase(left_bus_stops.begin()+i);
					break;
				}
				else if(this->is_neighbour(chosen_route.bus_stops[chosen_route.bus_stops.size()-1],left_bus_stops[i].idi) ){
					//std::cout << "Node is neighbour of end" << std::endl;
					chosen_route.bus_stops.push_back(left_bus_stops[i]);
					used_nodes.push_back(left_bus_stops[i]);
					left_bus_stops.erase(left_bus_stops.begin()+i);
					break;
				}
				else{
					//std::cout << "Node "  << left_bus_stops[i].idi << " is not neighbour of route" << std::endl;
					attempts_timeout--;
					//std::cout << "timeout: " << attempts_timeout << std::endl;
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

void Evolut1on::make_small_change(Solution *offspring){

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

bool Evolut1on::check_feasability2(Solution *sol){
        //Si hay un número mayor de rutas
		for(unsigned int i=0;i<this->route_types;i++){
                int count = 0;
                for(int r=0; r<sol->routes.size(); r++){
                        if( sol->routes[r].tipo_ruta == i) count++; 
                }
                if( count != this->routes_info[i][0]){std::cout << "Feasibility Error: Quantity" << std::endl; return false;}
        }
        //Chequear si las rutas tienen más nodos que el máximo o el mínimo.
        //Chequear si las rutas tienen ciclos
        for( unsigned int i=0;i<sol->routes.size();i++){
                               
               
                if( sol->routes[i].bus_stops.size() < (this->routes_info[sol->routes[i].tipo_ruta][1]) ){ std::cout << "Feasibility Error:  Min Length"<< std::endl; return false;}
                if( sol->routes[i].bus_stops.size() > (this->routes_info[sol->routes[i].tipo_ruta][2]) ){ std::cout << "Feasibility Error:  Max Length"<< std::endl; return false;}
               
                //Si hay ciclos, no es factible
                if( sol->routes[i].check_cycles_and_backtracks() ){ std::cout << "cyc" ; return false;}
        }
       
        //Si no es conexo, no es factible
        if( !sol->check_connectivity(this->p.get_size()) ){ std::cout << "Feasibility Error: Connectivity" << std::endl; return false;}
 
        return true;
};