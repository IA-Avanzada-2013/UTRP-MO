#include "common.h"
#include "BusStop.h"
#include <vector>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <cstdlib>
class Ants {
	
    

public:

    std::vector<BusStop> bus_stops;
    std::vector<std::vector<int> > feromonas;
    std::vector<Route> bestSolution;
    int size;
    int seed;
    int **demand;
    int **travel_times;
    int nAnts;
    int nIterations;
    int maxL;
    int minL;
    int q;
    float calidad;

    Ants(std::vector<BusStop> &bus_stops, int **&demand,int **&travel_times, int nAnts, int nIterations, int seed,std::vector<RouteInfo> routes_info);
	void initializePheromoneMatrix(int size);
	void printBusStops(std::vector<BusStop> bus_stops);
	void printPheromone();
	std::vector<BusStop> antMakeRoute();
	int getPositionInBusStops(BusStop bus);
	int verticalRoulette();
	int horizontalRoulette(std::vector<BusStop> stops,int p);
	void fillPheromone(std::vector<BusStop> stops, int cantidad);
	int random(int max);
	void getSolutions();
	void setBestSolution(std::vector<Route> rts);
    
};
