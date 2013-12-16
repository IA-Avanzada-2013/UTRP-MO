#include "Route.h"

Route::Route(void) {};
void Route::define_bus_type() {};

void Route::set_bus_stops(std::vector<BusStop> &bus_stops)
{
	this->bus_stops = bus_stops;
}

void Route::add_bus_stops(std::vector<BusStop> &bus_stops){
	
	for(int i = 0;i < bus_stops.size(); i++){
		this->bus_stops.push_back(bus_stops[i]);
		}
	} 

void Route::print_route(){
	for( int i = 0; i < this->bus_stops.size(); i++){
		std::cout << this->bus_stops[i].id;
		if(i < this->bus_stops.size()-1){
			std::cout << ", ";
			}
		}
		std::cout << std::endl;
	}

bool Route::check_cycles_and_backtracks()
{
	for (int i = 0; i < this->bus_stops.size()-1; ++i)
	{
		//std::cout << this->bus_stops[i].id << std::endl;
		//std::cout << "	vs" << std::endl;
		for (int j = i+1; j < this->bus_stops.size(); ++j)
		{
			//std::cout << this->bus_stops[j].id << std::endl;
			if (this->bus_stops[i] == this->bus_stops[j])
				return true;
		}
	}
    return false;
};
