#ifndef ROUTE_H
#define ROUTE_H

class Route {
    
    std::vector<BusStop> bus_stops;
    Bus bus_type; //o tipo de bus en esa ruta

public:
	
	Route(void);
    void define_bus_type();
};

#endif