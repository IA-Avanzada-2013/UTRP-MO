#include "common.h"
#include "BusStop.h"
#include "Problem.h"
#include "SolutionSet.h"
#include <vector>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <cstdlib>
#include <time.h>
class Ants {
	
    

public:

    std::vector<BusStop> bus_stops;
    std::vector<std::vector<double> > feromonas;
    std::vector<Route> bestSolution;
    std::vector<Route> actualSolution;
    Problem *problem;
    SolutionSet *st;
    time_t start;
    int size;
    int seed;
    int **demand;
    int **travel_times;
    int nAnts;
    int nIterations;
    int maxL;
    int minL;
    int q;
    int good;
    int bad;
    std::string instancia;
    float calidad1;
    float calidad2;

    Ants(std::vector<BusStop> &bus_stops, int **&demand,int **&travel_times, int nAnts, int nIterations,int good,int bad, int seed,std::vector<RouteInfo> routes_info,Problem *problem,std::string instancia);
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
    std::vector<BusStop> getPosiblesDestinos(BusStop bs);
    std::vector<BusStop> cleanDestinos(std::vector<BusStop> bs, std::vector<BusStop> stops);
    void pareto();
    
};
