#include "Route.h"

Route::Route(void) {};
void Route::define_bus_type() {};

void Route::set_bus_stops(std::vector<BusStop> &bus_stops)
{
	this->bus_stops = bus_stops;
}

bool Route::check_cycles_and_backtracks()
{
	for (int i = 0; i < this->bus_stops.size()-1; ++i)
	{
		for (int j = i+1; j < this->bus_stops.size(); ++j)
		{
			if (this->bus_stops[i] == this->bus_stops[j])
				return true;
		}
	}
    return false;
}
