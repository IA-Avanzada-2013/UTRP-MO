#ifndef PROBLEM_H
#define PROBLEM_H

#include "common.h"
#include "BusStop.h"

class Problem {

    //coordinate_t coordinate_array[SIZE];
    std::string name;
    
    int size;
    int **demand;
    int **travel_times; //-1 if infinite?

    std::vector<BusStop> bus_stops;

    void show_matrix(int **&matrix, int size);
    int routes_min, routes_max;

public:

    Problem(void);
    ~Problem(void);
    
    int get_demand(void); // calculates the total demand adding all demands

    void set_size(int size);
    void set_name(std::string name);
    std::string get_name(void);
    void set_demand(int **&demand);
    void set_travel_times(int **&travel_times);
    void set_bus_stops(std::vector<BusStop> &bus_stops);

    void show_bus_stops(void);
    void show_demand(void);
    void show_travel_times(void);

};

#endif
