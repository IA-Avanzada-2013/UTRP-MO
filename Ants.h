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
    int size;
    int **demand;
    int nAnts;
    int nIterations;

	Ants(std::vector<BusStop> &bus_stops, int **&demand, int nAnts, int nIterations);
	void initializePheromoneMatrix(int size);
	void printBusStops(std::vector<BusStop> bus_stops);
	void printPheromone();
	std::vector<BusStop> antMakeRoute();
	int getPositionInBusStops(BusStop bus);
	int verticalRoulette();
	int horizontalRoulette(std::vector<BusStop> stops,int p);
	void fillPheromone(std::vector<BusStop> stops);
    
};
