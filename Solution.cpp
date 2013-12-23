#include "Solution.h"
#include "Route.h"
#include "ShortestRoute.h"


Solution::Solution(void) {};
void Solution::set_routes(std::vector<Route> &routes)
{
	this->routes = routes;
}
void Solution::quality(){};         //Calcula alterando el valor de fo1 y fo2
bool Solution::check_feasability(){};
int Solution::setFO1(ShortestRoute *sr, int **&demand){
    float numerador=0.0;
    float denominador=0.0;
    float transferencias=0.0;

    for (int i = 0; i < sr->getSize(); i++)
    {
        for (int j = 0; j < sr->getSize(); j++)
        {
            
                numerador+=demand[i][j]*sr->distance(i,j);
                denominador+=demand[i][j];
                transferencias+=5*sr->getTransfers(i,j,this->routes);

        }
    }    
    //std::cout << "\nFO:" << numerador/denominador << "\n";
    this->fo1 = (numerador/denominador)+transferencias;
    return (numerador/denominador)+transferencias;
    //this->fo1 = (numerador/denominador);
    //return (numerador/denominador);
};

int Solution::setF02(int size, int **&travel_times) {
	float time=0.0;
	int it_ruta;
	
	//iteracion sobre todas las rutas
	for(it_ruta=0;it_ruta<this->routes.size();it_ruta++) {
		for(int i=0;i<size;i++) {
			for(int j=i;j<size;j++) {
				//if(this->routes[it_ruta].check_edge(i,j)) {
					time+=travel_times[i][j];
				//}
			}
		}
	}
	
	this->fo2=time;
	return time;
}

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

bool Solution::print_solution_routes(){
	std::string filename;
	filename = "Results/SolutionRoutes.txt";
	std::ofstream sol(filename.c_str());

	for (int i = 0; i < this->routes.size() ; i++){
		for( int j = 0; j < this->routes[i].bus_stops.size(); j++){
			sol << this->routes[i].bus_stops[j].id;
			if(j < this->routes[i].bus_stops.size()-1){
				sol << ", ";
			}
		}
		sol << std::endl;
	}  
  	sol.close();
};
