#include "Solution.h"
#include "Route.h"
#include "ShortestRoute.h"


Solution::Solution(void) 
{
	
}

void Solution::set_routes(std::vector<Route> &routes)
{
	this->routes = routes;
}

//Calcula alterando el valor de fo1 y fo2
void Solution::setQuality(float alpha, float beta)
{
	float aptitud;
	aptitud = alpha*this->fo1+beta*this->fo2;
	this->quality=aptitud;
}
         
bool Solution::check_feasability(){};
float Solution::setFO1(ShortestRoute *sr, int **&demand){
    float numerador=0.0;
    float denominador=0.0;
    float transferencias=0.0;

    for (int i = 0; i < (sr->getSize()-1); i++)
    {
        for (int j = i+1; j < sr->getSize(); j++)
	{
		//std::cout<<"Transferencias ("<<i+1<<","<<j+1<<"):"<< sr->getTransfers(i,j,this->routes)<<std::endl;
		//getchar();
                numerador+=demand[i][j]*(sr->distance(i,j) + 5*sr->getTransfers(i,j,this->routes));
                denominador+=demand[i][j];

        }
    }    
    //std::cout << "\nFO:" << numerador/denominador << "\n";
    //cout << "numerador " << numerador << endl;
    //cout << "denominador " << denominador << endl;
    //cout << "division" << numerador/denominador << endl;
    this->fo1 = (numerador/denominador);
    return (numerador/denominador);
    //this->fo1 = (numerador/denominador);
    //return (numerador/denominador);
};

float Solution::setF02(int size, int **&travel_times) {
	float time=0.0;
	int it_ruta;
	
	//iteracion sobre todas las rutas
	for(it_ruta=0;it_ruta<this->routes.size();it_ruta++) {
		for(int i=0;i<size;i++) {
			for(int j=i;j<size;j++) {
				if(this->routes[it_ruta].check_edge(i,j)) {
					time+=travel_times[i][j];
				}
			}
		}
	}
	
	this->fo2=time;
	return time;
}

bool Solution::check_connectivity(int size){

	Route *set = new Route();
	set->set_bus_stops(this->routes[0].bus_stops);
	
	int i, flag = 0, added_sum = 1, added_sum_old = 0;
	int added [this->routes.size()];
	std::fill_n(added,this->routes.size(),0);
	added [0] = 1;
	
	while(added_sum_old < added_sum){
		added_sum_old = added_sum;
		for (i = 1; i < this->routes.size() ; i++){
			//std::cout << "revisando ruta: " << i << std::endl;
			flag = 0;
			if(added[i] == 0){
				for (int j= 0; j < this->routes[i].bus_stops.size();j++){
					for (int k = 0; k < set->bus_stops.size(); k++){
						//std::cout << "comparando paradas " << this->routes[i].bus_stops[j].id << " y " << set->bus_stops[k].id << std::endl;
						if(this->routes[i].bus_stops[j] == set->bus_stops[k]){
							set->add_distinct_bus_stops(this->routes[i].bus_stops);
							added [i] = 1;
							added_sum +=1;
							flag = 1;
							break;
						}
					}
					if(flag == 1){
						break;
					}
				}
			}
		}
	}

	// std::cout << "p->size: " << size << std::endl;
	// std::cout << "set size: " << set->bus_stops.size() << std::endl;
	// std::cout << "Connected Bus Stops: " << std::endl;
	// set->print_route();
	if(size == set->bus_stops.size()){
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
			sol << this->routes[i].bus_stops[j].idi+1;
			if(j < this->routes[i].bus_stops.size()-1){
				sol << ", ";
			}
		}
		sol << std::endl;
	}  
  	sol.close();
};
