#include "Problem.h"


Problem::Problem(void) {};
Problem::~Problem(void) {};
int Problem::get_demand(void) {}; // calculates the total demand adding all demands

void Problem::set_size(int size)
{
	this->size = size;
}

void Problem::set_demand(int **&demand)
{
	this->demand = demand;
}

void Problem::set_travel_times(int **&travel_times)
{
	this->travel_times = travel_times;
}

void Problem::set_bus_stops(std::vector<BusStop> &bus_stops)
{
	this->bus_stops = bus_stops;
}

void Problem::show_bus_stops(void)
{
	for (std::vector<BusStop>::iterator it =  this->bus_stops.begin(); it!= this->bus_stops.end(); ++it)
	{
		std::cout << "BusStop \"" << it->id << "\": (" << it->x << ", " << it->y << ")" << std::endl;
    }
}

void Problem::show_demand(void)
{
	::show_matrix(this->demand, this->size);
}

void Problem::show_travel_times(void)
{
	::show_matrix(this->travel_times, this->size);
}
