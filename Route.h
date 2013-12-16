#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include "common.h"
#include "BusStop.h"


class Route {

public:

    std::vector<BusStop> bus_stops;
    // Bus bus_type; //o tipo de bus en esa ruta
	
	Route(void);
	void set_bus_stops(std::vector<BusStop> &bus_stops);
	void add_bus_stops(std::vector<BusStop> &bus_stops);
	void print_route();
    void define_bus_type();
    bool check_cycles_and_backtracks(void);
};

#endif
