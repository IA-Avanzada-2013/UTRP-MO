#include "Solution.h"
#include "Route.h"

Solution::Solution(void) {};
void Solution::set_routes(std::vector<Route> &routes)
{
	this->routes = routes;
}
void Solution::quality(){};         //Calcula alterando el valor de fo1 y fo2
bool Solution::check_feasability(){};

bool Solution::check_connectivity(){

	Route *set = new Route();
	set->set_bus_stops(this->routes[0].bus_stops);
	
	int sum = this->routes[0].bus_stops.size();
	int flag = 0;
	
	for (int i = 1; i < this->routes.size() ; i++){
		//std::cout << "revisando ruta: " << i << std::endl;
		flag = 0;
		for (int j= 0; j < this->routes[i].bus_stops.size();j++){
			for (int k = 0; k < set->bus_stops.size(); k++){
				//std::cout << "comparando paradas " << this->routes[i].bus_stops[j].id << " y " << set->bus_stops[k].id << std::endl;
				if(this->routes[i].bus_stops[j] == set->bus_stops[k]){
					set->add_bus_stops(this->routes[i].bus_stops);
					flag = 1;
					break;
					}
				}
				if(flag == 1){
					break;
				}
			}
		sum = sum+ this->routes[i].bus_stops.size();
		}
	
	//std::cout << "sum: " << sum << std::endl;
	//std::cout << "set size: " << set->bus_stops.size() << std::endl;
	if(sum == set->bus_stops.size()){
		//std::cout << "Connected Bus Stops: " << std::endl;
		//set->print_route();
		return true;
		}

	return false;
};
