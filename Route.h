#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include "common.h"
#include "BusStop.h"

using namespace std;

class Route {

public:

    std::vector<BusStop> bus_stops;
    int tipo_ruta;
    // Bus bus_type; //o tipo de bus en esa ruta
	
	Route(void);
	void set_bus_stops(std::vector<BusStop> &bus_stops);
    void add_bus_stops(std::vector<BusStop> &bus_stops);
    void add_distinct_bus_stops(std::vector<BusStop> &bus_stops);
    void print_route();
    void define_bus_type(int);
    bool check_cycles_and_backtracks();
    //Verifica que el arco entre a y b se encuentra en la ruta.
    bool check_edge(int id_a, int id_b);
    //Verifica si un nodo esta en la ruta.
    bool find_node(int id_a);
    //Verifica si dos nodos estan en la ruta.
    bool find_nodes(int id_a, int id_b);
};

#endif
