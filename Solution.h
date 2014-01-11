#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <fstream>
#include "common.h"
#include "Route.h"
#include "ShortestRoute.h"
#include <vector>

class Solution {
	    
public:
	float fo1,fo2; // stores the result of the objective functions for efficiency
	float quality;
	std::vector<Route> routes;
	Solution(void);
	void set_routes(std::vector<Route> &routes);
	void setQuality(float,float);         //Calcula alterando el valor de fo1 y fo2
	bool check_feasability();
	bool check_connectivity(int size);
	float setFO1(ShortestRoute *sr, int **&demand);
	float setF02(int size, int **&travel_times);

	bool print_solution_routes();
    
};

#endif
