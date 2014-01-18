#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"
#include "Evolut1on.h"
#include <algorithm>

Evolut1on::Evolut1on(Problem p, int seed, std::vector<RouteInfo> routes_info, int solset_size, float prob_mutation, float prob_crossover, int fxeval){
	this->p = p;
    this->seed=seed;
    srand ( this->seed );
    this->route_types = routes_info.size();
    this->prob_crossover = prob_crossover;
    this->prob_mutation = prob_mutation;
    this->fxeval = fxeval;

    //routes_info row=id, col1=quantity, col2=min, col3=max
	this->routes_info = new int*[routes_info.size()];
	for (int i = 0; i < routes_info.size(); ++i){
		this->routes_info[i] = new int[3];
		this->routes_info[i][0]=routes_info[i].quantity;
		this->routes_info[i][1]=routes_info[i].min_length; 
		this->routes_info[i][2]=routes_info[i].max_length;
	}

	//calculate total routes needed
	this->number_of_routes=0;
	for (int i = 0; i < this->route_types; ++i)
		this->number_of_routes += this->routes_info[i][0];

    //generate feasible routes sets
    std::cout << " Generating Initial Solutions" << std::endl;
    Solution new_sol;
    bool dominates=false;
    for (int i = 0; i < solset_size; ++i)
    {
    	new_sol = generate_feasible_route_set(routes_info)[0];
    	dominates=false;
    	for (int j = 0; j < this->result.solutions.size(); ++j){
    		if(this->domination(&new_sol,this->result.solutions[j])){
    			dominates = true;
    			this->result.solutions[j]=new_sol;
    			this->result.solutions[j].id = j;
    			i--;
    			break;
    		}
    	}
    	if(!dominates){
    		this->result.solutions.push_back(new_sol);
    		this->result.solutions[i].id = i;
    	}
    }
    std::cout << solset_size<< " Solutions Generated!" << std::endl;
    this->set_best_routes();
    //this->result.print_solution_set();
    //exec the algorithm

    this->seamo2();
	this->result.print_solution_set();
	float hypervolume = hv(&this->result, &this->p);
    std::cout<<"Hipervolume: "<<hypervolume<<std::endl;
};

Evolut1on::~Evolut1on(void){
}

Solution *Evolut1on::generate_feasible_route_set(std::vector<RouteInfo> routes_info){
	Solution *sol = new Solution();
	std::vector<BusStop> used_nodes;
	std::vector<Route> route_set;

	int diff_size = 0;
	bool used=false,is_not_feasible=true;
	int tmp_route_size, tmp_route_type;
	
	Route *tmp_route;
	while(is_not_feasible){
		for (int i = 0; i < this->number_of_routes; ++i){
			//std::cout << "Generating route nº:" << i << std::endl;
			//get the route type assigned to de i'th route
			int routes_so_far=0;
			tmp_route_type = this->get_route_type(i);
			diff_size = this->routes_info[tmp_route_type][2]-this->routes_info[tmp_route_type][1];
			tmp_route_size = rand() % diff_size + this->routes_info[tmp_route_type][1];
			//std::cout << "tipo_ruta: " <<tmp_route_type << ", tamaño: "<<tmp_route_size<< std::endl;
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
	    if(this->check_feasability2(sol)){
	    	//std::cout << "Its Feasible!" << std::endl;
	    	this->set_fs(sol);
			//std::cout<< "FO's: (" << sol->fo1 << "," <<  sol->fo2 << ") # press enter to continue" <<std::endl;
			//std::cin.get();
	    	is_not_feasible=false;
	    }
	    else{
	    	//std::cout << "Its NOT Feasible:" << std::endl;	
	    	if(this->repair_routeset(sol)){
	    		//std::cout << "Route Repaired!" << std::endl;
	    		if(this->check_feasability2(sol)){
	    			//std::cout << "Now Its Feasible =)" << std::endl;
					this->set_fs(sol);
					//std::cout<< "FO's: (" << sol->fo1 << "," <<  sol->fo2 << ")  # press enter to continue" <<std::endl;
					//std::cin.get();
					is_not_feasible=false;
	    		}
	    		else{
		    		//std::cout << "Repair Failed =(" << std::endl;
		    		route_set.clear();
		    		delete sol;
		    		sol = new Solution();
	    		}
					
	    	}
	    	else{
	    		//std::cout << "Repair Failed =(" << std::endl;
	    		route_set.clear();
	    		delete sol;
	    		sol = new Solution();
	    	}
	    } 
	}
	//std::cout << "Solution Added!" << std::endl;
	return sol;
}

Route *Evolut1on::generate_route(int tmp_route_size,std::vector<Route> routes_so_far, int tmp_route_type){
	//std::cin.get();
	Solution actual_routeset;
	actual_routeset.set_routes(routes_so_far);
	std::vector<BusStop> used_nodes = this->get_specific_nodes(&actual_routeset,true);
	std::vector<BusStop> left_bus_stops= this->get_specific_nodes(&actual_routeset,false);

	//std::cout << "Nodes Used So Far: " << used_nodes.size();
	//this->print_temp_route(used_nodes);
	
	//std::cout << "Free & Unreachable Nodes Left: " << left_bus_stops.size();	
	//this->print_temp_route(left_bus_stops);


	//std::cout << "Route Generator Started, expected size :"<<tmp_route_size << std::endl;
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
		//std::cin.get();
		//if its the first node of the route
		if(tmp_bus_stop_route.size()==0){
			//std::cout << "Adding First Node"<< std::endl;
			//if its the first route
			if(used_nodes.size()==0){
				tmp_route_start = rand() % left_bus_stops.size();
				previous_node=tmp_route_start;
				tmp_bus_stop_route.push_back(left_bus_stops[tmp_route_start]);
				//std::cout << "Adding First Node: "<< previous_node<<std::endl;
			}
			else{
				//random already used node (with cool addon)
				tmp_route_start = this->nice_neighbour(used_nodes,left_bus_stops);
				if(tmp_route_start==-1)
					tmp_route_start = rand() % used_nodes.size();
				previous_node=used_nodes[tmp_route_start].idi;
				tmp_bus_stop_route.push_back(used_nodes[tmp_route_start]);
				//std::cout << "Adding First Node: "<<used_nodes[tmp_route_start].idi<<std::endl;
			}
		}

		//std::cout << "Finding neighbours"<< std::endl;
		neighbours = this->get_neighbour_bus_stops(previous_node,tmp_bus_stop_route,left_bus_stops);
		//this->print_temp_route(neighbours);
		if (neighbours.size() != 0){
			tmp_neighbour = this->nice_neighbour(neighbours,left_bus_stops);
			//std::cout << "NN position: "<< tmp_neighbour<<std::endl;
			if(tmp_neighbour==-1)
				tmp_neighbour = rand() % neighbours.size();
			previous_node = neighbours[tmp_neighbour].idi;
			tmp_bus_stop_route.push_back(neighbours[tmp_neighbour]);
			//std::cout << "Adding Found neighbour: "<< previous_node<<std::endl;
		}
		else{
			//std::cout << "No neighbours found, looking in full set"<< std::endl;
			neighbours = this->get_neighbour_bus_stops(previous_node,tmp_bus_stop_route,this->p.get_bus_stops());
			//this->print_temp_route(neighbours);
			if (neighbours.size() != 0){
				tmp_neighbour = this->nice_neighbour(neighbours,left_bus_stops);
				//std::cout << "NN: "<< tmp_neighbour<<std::endl;
				if(tmp_neighbour==-1)
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
	Solution parent1,parent2,*offspring;
	int rand_id,eval_countdown=this->fxeval;
	bool condition_satisfied=false;
	while(!condition_satisfied){
		//std::cout<<"Seamo2 Starting"<<std::endl;
		for (int i = 0; i < this->result.solutions.size(); ++i)
		{
			parent1 = this->result.solutions[i];
			do {rand_id = rand() % this->result.solutions.size();}while(rand_id!=i);
			parent2 = this->result.solutions[rand_id];
			if((float)rand() < this->prob_crossover)
				offspring = this->crossover(parent1,parent2);
			else
				offspring = &parent1;
			//std::cout<<"Crossover Check!"<<std::endl;
			if((float)rand() < this->prob_mutation)
				this->mutate(offspring);//apply mutation

			if(this->check_feasability2(offspring)){
				//std::cout<<"Mutation is Feasible!"<<std::endl;
				this->set_fs(offspring);
				eval_countdown --;
				if(this->check_duplicate_route_set(&this->result, offspring)){
					std::cout<<"Mutation is a duplie!"<<std::endl;
				}
				else if(this->domination(offspring,parent1)){
					offspring->id=parent1.id;
					this->result.solutions[parent1.id]=*offspring;

				}
				else if(this->domination(offspring,parent2)){
					offspring->id=parent2.id;
					this->result.solutions[parent2.id]=*offspring;
				}
				else if(offspring->fo1 < this->result.solutions[this->best_routeset_so_far_o1].fo1){
					if(parent1.id != best_routeset_so_far_o1 && parent1.id != best_routeset_so_far_o2){
						offspring->id=parent1.id;
						this->result.solutions[parent1.id]=*offspring;
					}
					else{
						if(parent2.id != best_routeset_so_far_o1 && parent2.id != best_routeset_so_far_o2){
							offspring->id=parent2.id;
							this->result.solutions[parent2.id]=*offspring;
						}
						else{
							//std::cout << "both parents contain best route so far =(" << std::endl;
						}
					}
				}
				else{
					bool dominates = false;
					int dominated_id;
					for (int j = 0; j < this->result.solutions.size(); ++j){
						if(this->domination(offspring,this->result.solutions[j])){
							dominated_id=j;
							dominates=true;
							break;
						}
					}
					if(dominates){
						offspring->id=this->result.solutions[dominated_id].id;
						this->result.solutions[dominated_id]=*offspring;
					}
				}

			}
		}
		if(eval_countdown==0)
			condition_satisfied=true;	
	}
}

bool Evolut1on::domination(Solution *offspring, Solution parent){
	bool result = false;
	if(offspring->fo1 < parent.fo1 && offspring->fo2 < parent.fo2)
		result = true;
	return result;
}

Solution *Evolut1on::crossover(Solution P1, Solution P2){
	Solution *result = new Solution();
	std::vector<BusStop> used_nodes,left_bus_stops;
	std::vector<Route> route_set;
	int tmp_nice_route,tmp_route_type=0;
	//first route
	route_set.push_back(P1.routes[this->nice_route(P1.routes,used_nodes,left_bus_stops,tmp_route_type,route_set)]);
	for (int i = 1; i < this->number_of_routes; ++i){
		tmp_route_type=this->get_route_type(i);
		//std::cout << "Tipo de Ruta: " << tmp_route_type << std::endl;
		result->set_routes(route_set);
		used_nodes = this->get_specific_nodes(result,true);
		left_bus_stops= this->get_specific_nodes(result,false);
		if(i%2){//even node
			tmp_nice_route= this->nice_route(P1.routes,used_nodes,left_bus_stops,tmp_route_type,route_set);
			//std::cout << "nice route = " << tmp_nice_route << " de tipo: " << P1.routes[tmp_nice_route].tipo_ruta<<std::endl;
			route_set.push_back(P1.routes[tmp_nice_route]);
		}
		else{//odd node
			tmp_nice_route= this->nice_route(P2.routes,used_nodes,left_bus_stops,tmp_route_type,route_set);
			//std::cout << "nice route = " << tmp_nice_route << " de tipo: " << P2.routes[tmp_nice_route].tipo_ruta<<std::endl;
			route_set.push_back(P2.routes[tmp_nice_route]);
		}
	}
	result->set_routes(route_set);
	if(this->check_feasability2(result)){
    	this->set_fs(result);
    }
    else{
    	//std::cout << "Its NOT Feasible:" << std::endl;
    	if(this->repair_routeset(result)){
    		//std::cout << "Route Repaired!" << std::endl;
    		if(this->check_feasability2(result)){
    			//std::cout << "Now Its Feasible =)" << std::endl;
				this->set_fs(result);
    		}
    	}
    	else
    		std::cout << "Repair Failed =(" << std::endl;
    }
	return result;
}

void *Evolut1on::mutate(Solution *offspring){
	bool mutated = false;
	int countdown=10; //10 attempts no select mutating node
	
	//std::cout<<"Starting Mutation!"<<std::endl;
	std::vector<BusStop> v = this->p.get_bus_stops();
	//selection of node
	int idx = v[rand() % v.size()].idi;
	int route1_type,route2_type,route1_id,route2_id;
	std::vector<BusStop> route1, route2,nice_neighbours2;
	int i, position=1000,found=0;
	while(!mutated){
		//seleccion dos rutas que tienen el nodo
		for ( i= 0; i < offspring->routes.size(); ++i){
			if(this->find_node(offspring->routes[i].bus_stops,idx)){
				found ++;
				if(found==1){
					route1 = offspring->routes[i].bus_stops;
					route1_type = offspring->routes[i].tipo_ruta;
					route1_id = i;
				}
				if(found==2){
					route2 = offspring->routes[i].bus_stops;
					route2_type = offspring->routes[i].tipo_ruta;
					route2_id = i;
					break;
				}
			}
		}
		//std::cout<<"Routes Chosen! types: ("<< route1_type << ","<< route2_type<< ")"<<std::endl;
		if(found==2){
			//swap routes until node is reached
			std::vector<BusStop> route1m, route2m;
			//swap previous nodes
			for ( i = 0; i < route2.size(); ++i){ 
				route1m.push_back(route2[i]);
				if(route2[i].idi==idx)
					break;
			}
			for ( i = 0; i < route1.size(); ++i){ 
				route2m.push_back(route1[i]);
				if(route1[i].idi==idx)
					break;
			}
			//std::cout<<"Swapped first side!"<<std::endl;
			//fill in rest of routes
			for ( i = 0; i < route1.size(); ++i){ 
				if(route1[i].idi==idx)
					position=i;
				if(position < i){
					if(this->find_node(route1m,route1[i].idi)) //avoid cycles
						break;
					if(route1m.size() == this->routes_info[route1_type][2])//avoid larger size
						break;
					route1m.push_back(route1[i]);
				}
			}	
			position = 1000;
			for ( i = 0; i < route2.size(); ++i){ 
				if(route2[i].idi==idx)
					position=i;
				if(position < i){
					if(this->find_node(route2m,route2[i].idi)) //avoid cycles
						break;
					if(route2m.size() == this->routes_info[route2_type][2])//avoid larger size
						break;
					route2m.push_back(route2[i]);
				}
			}
			//std::cout<<"Routes mixed!"<<std::endl;		
			//if the route was cut, and its actual length is smaler than min, add extra nodes till min is reached.
			if(route1m.size() < this->routes_info[route1_type][1]){
				route1m = this->refill_route(route1m, this->routes_info[route1_type][1]-route1m.size());
			}
			if(route2m.size() < this->routes_info[route2_type][1]){
				route2m = this->refill_route(route2m ,this->routes_info[route2_type][1]-route2m.size());
			}
			//std::cout << "Mutation Result:" <<std::endl;
			//this->print_temp_route(offspring->routes[route1_id].bus_stops);
			//std::cout << "is now:" <<std::endl;
			offspring->routes[route1_id].bus_stops = route1m;
			//this->print_temp_route(offspring->routes[route1_id].bus_stops);
			//std::cout << "&" <<std::endl;
			//this->print_temp_route(offspring->routes[route2_id].bus_stops);
			offspring->routes[route2_id].bus_stops = route2m;
			//std::cout << "is now:" <<std::endl;
			//this->print_temp_route(offspring->routes[route2_id].bus_stops);
		}
		else{
			countdown --;
			if(countdown==0)
				break;
		}
	}
}

//refills route until min length is reached
std::vector<BusStop> Evolut1on::refill_route(std::vector<BusStop> in, int diff){
	std::vector<BusStop> neigh;
	int top=1000,top_id=-1;
	for (int i = 0; i < diff; ++i){
		neigh = this->get_neighbour_bus_stops(in[in.size()-1].idi,in,this->p.get_bus_stops());
		//get the less-travel-time neighbour
		for (int j = 0; j < neigh.size(); ++j){
			if(this->p.get_travel_times()[neigh[j].idi][in[in.size()-1].idi]<top){
				top=this->p.get_travel_times()[neigh[j].idi][in[in.size()-1].idi];
				top_id=j;
			}

		}
		if(top_id != -1)
			in.push_back(neigh[top_id]);
	}
	return in;
}

//returns id of route best fit
int Evolut1on::nice_route(std::vector<Route> route_set,std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops, int tmp_route_type,std::vector<Route> actual_route_set){
	int result=-1,tmp_count;
	float best_so_far=0;
	//std::cout <<"Escogiendo Ruta de tipo:" <<tmp_route_type <<std::endl;
	//returns the id of route which contains more unused nodes 
	for (int i = 0; i < route_set.size(); ++i){
		if(route_set[i].tipo_ruta==tmp_route_type){
			tmp_count=0;
			for (int j = 0; j < route_set[i].bus_stops.size(); ++j){
				if(this->find_node(left_bus_stops,route_set[i].bus_stops[j].idi)){
					//std::cout << "node found " <<std::endl;
					tmp_count++;
				}
			}
			//std::cout << "nice route "<< i << ": " << (float)tmp_count/route_set[i].bus_stops.size() <<std::endl;
			if(((float)tmp_count/route_set[i].bus_stops.size()<1) && ((float)tmp_count/route_set[i].bus_stops.size()>best_so_far)){
				
				//std::cout<<"yay!!"<<std::endl;		
				best_so_far = (float)tmp_count/route_set[i].bus_stops.size();
				result=i;
			
			}
		}
	}
	if(result==-1){
		int tmp_route_id;
		bool flag = true;
		do{
			tmp_route_id=rand()%route_set.size();
			if(!this->find_route(actual_route_set,route_set[tmp_route_id])){
				//std::cout << "esta no!" << std::endl;
				if(route_set[tmp_route_id].tipo_ruta==tmp_route_type){
					flag=false;
					result=tmp_route_id;
				}
			}
		}
		while(flag);
	}
		
	//std::cout<<"nice route: "<< result <<  " type: " << route_set[result].tipo_ruta << "/" << tmp_route_type<< std::endl;		
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

//returns true if Rout belongs actual route set
bool Evolut1on::find_route(std::vector<Route>actual, Route buscada){
	bool result=false;
	for (int i = 0; i < actual.size(); ++i){
		if(actual[i].tipo_ruta==buscada.tipo_ruta && actual[i].bus_stops.size()==buscada.bus_stops.size()){
			result=true;
			for (int j = 0; j < actual[i].bus_stops.size(); ++j){
				if(actual[i].bus_stops[j].idi==buscada.bus_stops[j].idi)
					result=false;
			}
		}
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
	//std::cout << "neighbours found for: " << previous_node;
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

//selects a node which is actually in a route, neighbour to an unused node... returns position in array
int Evolut1on::nice_neighbour(std::vector<BusStop> used_nodes, std::vector<BusStop> left_bus_stops){
	int nice_neighbour =-1;
	int i = rand() % used_nodes.size();
	for (int j = 0; j < left_bus_stops.size(); ++j){
		//std::cout << "comparando vecinidad " << used_nodes[i].idi <<"," << left_bus_stops[j].idi<< std::endl;
		if(this->is_neighbour(used_nodes[i],left_bus_stops[j].idi)){
			nice_neighbour =  used_nodes[i].idi;
			//std::cout<<"Nice neighbour found: "<< nice_neighbour<<std::endl;
			return i;
		}
	}
		
	return nice_neighbour;
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

int Evolut1on::get_route_type(int i){
	int tmp_route_type=0,routes_so_far=0;
	for (int j = 0; j < this->route_types; ++j)
	{
		routes_so_far+=this->routes_info[j][0];
		if (i<routes_so_far){
			
			tmp_route_type = j;
			break;
		}
	}
	return tmp_route_type;
}

//status = true if they are used nodes, false if not
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
					//if(result[k].idi==routeset->routes[i].bus_stops[j].idi && this->is_reached(result[k].idi,routeset->routes))
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
	//std::cin.get();
	std::vector<BusStop> used_nodes = this->get_specific_nodes(unfeasible_routeset,true);
	std::vector<BusStop> left_bus_stops= this->get_specific_nodes(unfeasible_routeset,false);
	bool result=false;
	int attempts_timeout=0;
	for (int i = 0; i < this->route_types; ++i)
		attempts_timeout += this->routes_info[i][0];
	attempts_timeout = attempts_timeout*left_bus_stops.size();

	bool possibilities_exhausted = false;
	Route *chosen_route;
	//std::cout << "Free & Unreachable Nodes Left: " << left_bus_stops.size();
	//this->print_temp_route(left_bus_stops);
	while((left_bus_stops.size()!=0) && !possibilities_exhausted){
		//std::cout << "Free Nodes Left: " << left_bus_stops.size();	
		//this->print_temp_route(left_bus_stops);
		// std::cout << "Starting repair attempt" << std::endl;
		chosen_route=&unfeasible_routeset->routes[rand() % unfeasible_routeset->routes.size()];
		//chosen_route.print_route();
		if(chosen_route->bus_stops.size()<this->routes_info[chosen_route->tipo_ruta][2]){
			//std::cout << "Route can accept extra nodes" << std::endl;
			for (int i = 0; i < left_bus_stops.size(); ++i)	{
				//std::cout << "There is/are " << left_bus_stops.size() << " bus stop(s) left" <<std::endl; 
				//std::cout << "Checking left node "<< left_bus_stops[i].idi << std::endl;
				if(this->is_neighbour(chosen_route->bus_stops[chosen_route->bus_stops.size()-1],left_bus_stops[i].idi) ){
					if(this->is_neighbour(chosen_route->bus_stops[0],left_bus_stops[i].idi) ){
						//std::cout << "Node is Neighbour of the beginning" << std::endl;
						chosen_route->bus_stops.insert(chosen_route->bus_stops.begin(), left_bus_stops[i]);
						used_nodes.push_back(left_bus_stops[i]);
						left_bus_stops.erase(left_bus_stops.begin()+i);
						break;
					}
					else if(!this->find_node(chosen_route->bus_stops,left_bus_stops[i].idi)){
						//std::cout << "Node is neighbour of end" << std::endl;
						//this->print_temp_route(chosen_route->bus_stops);
						chosen_route->bus_stops.push_back(left_bus_stops[i]);
						
						if(chosen_route->bus_stops.size() > this->routes_info[chosen_route->tipo_ruta][2]){
							chosen_route->bus_stops.erase(chosen_route->bus_stops.begin(), chosen_route->bus_stops.begin() + 1);
						}
						//this->print_temp_route(chosen_route->bus_stops);
						used_nodes.push_back(left_bus_stops[i]);
						left_bus_stops.erase(left_bus_stops.begin()+i);
						break;
					}
				}
				else{
					//std::cout << "Node "  << left_bus_stops[i].idi << " is not neighbour of route" << std::endl;
					attempts_timeout--;
					//this->print_temp_route(left_bus_stops);
					//std::cout << "timeout: " << attempts_timeout << std::endl;
					if(attempts_timeout<0)
						possibilities_exhausted=true;
				}
			}
		}
	}
	if(!possibilities_exhausted){
		result=true;
		//this->print_temp_route(left_bus_stops);
		//std::cout << "timeout2: " << attempts_timeout << std::endl;
	}
	return result;
}

bool Evolut1on::is_reached(int idx, std::vector<Route> routes){
	bool result=false;
	for (int i = 0; i < routes.size(); ++i){
		for (int j = 1; j < routes[i].bus_stops.size(); ++j){
			if(routes[i].bus_stops[j].idi == idx)
				result=true;
		}
	}

	return result;
}

bool Evolut1on::check_duplicate_routes(Solution *routeset, Route *route){
	bool result=false;
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

bool Evolut1on::check_duplicate_route_set(SolutionSet *solset, Solution *offspring){
	bool result=false;
	for (int i = 0; i < solset->solutions.size(); ++i){
		result=true;
		for (int j = 0; j < offspring->routes.size(); ++j){
			if(!this->check_duplicate_routes(&solset->solutions[i],&offspring->routes[j])){
				result=false;
				break;
			}
		}
	}
	return result;
}

bool Evolut1on::check_feasability2(Solution *sol){
        //Si hay un número mayor de rutas
		int count;
		for(unsigned int i=0;i<this->route_types;i++){
			//std::cout << "Checking Route Type: "<< i << std::endl;	
            count = 0;
            for(int r=0; r<sol->routes.size(); r++){
                    if( sol->routes[r].tipo_ruta == i) count++; 
            }
            if( count != this->routes_info[i][0]){
            	//std::cout << "Feasibility Error: Quantity  (" << count << "/" << this->routes_info[i][0]  << ")" << std::endl; 
            	return false;
            }
        }
        //Chequear si las rutas tienen más nodos que el máximo o el mínimo.
        //Chequear si las rutas tienen ciclos
        for( unsigned int i=0;i<sol->routes.size();i++){
                if( sol->routes[i].bus_stops.size() < (this->routes_info[sol->routes[i].tipo_ruta][1]) ){ 
                	//std::cout << "Feasibility Error:  Min Length"<< std::endl; 
                	return false;
                }
                if( sol->routes[i].bus_stops.size() > (this->routes_info[sol->routes[i].tipo_ruta][2]) ){ 
                	//std::cout << "Feasibility Error:  Max Length"<< std::endl; 
                	return false;
                }
               
                //Si hay ciclos, no es factible
            if( sol->routes[i].check_cycles_and_backtracks() ){ 
        		//std::cout << "Feasibility Error:  Cycles"<< std::endl;  
        		return false;
        	}
        }
       
        //Si no es conexo, no es factible
        if( !sol->check_connectivity(this->p.get_size()) ){ 
        	//std::cout << "Feasibility Error: Connectivity" << std::endl; 
        	return false;
        }
 
        return true;
};