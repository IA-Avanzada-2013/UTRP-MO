#include <iostream>
#include <vector>

#include <getopt.h>
#include <cstdlib>

#include "common.h"
#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "BusStop.h"
#include "Route.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"
#include "Ants.h"

void intro(void)
{
	std::string s = 	"____ __________________________________  _____   ________   		\n"
						"|    |   \\__    ___/\\______   \\______   \\/     \\  \\_____  \\ \n"
						"|    |   / |    |    |       _/|     ___/  \\ /  \\  /   |   \\ 	\n"
						"|    |  /  |    |    |    |   \\|    |  /    Y    \\/    |    \\	\n"
						"|______/   |____|    |____|_  /|____|  \\____|__  /\\_______  /	\n"
						"                            \\/                 \\/         \\/ 	\n";

	s += ">>> UTRP Multi Objective C++ Approach by UTFSM Students :D <<< ";
	std::cout << s << std::endl;
}

void usage(void)
{
	std::string s = 	"Usage: ./utrpmo -i INSTANCE_PREFIX -s SEED -r ROUTE_INFO\n"
						"For example: ./utrpmo -i instances/Mandl -s 12345 -r \"10:2:7;12:3:8\" \n";

	std::cout << s << std::endl;
}

int main(int argc, char **argv)
{

	intro();

	int size;
    char c;

    int opt_seed = -1;
    std::string opt_route_type;
    std::string opt_instance_prefix;
    std::vector<RouteInfo> routes_info;


    while ((c = getopt(argc, argv, "i:s:r:")) != -1)
    switch (c)
    {
    	case 'i':
    		opt_instance_prefix = optarg;
    		break;
	    case 's':
	        opt_seed = atoi(optarg);
	        break;
   	    case 'r':
   	    {
	        opt_route_type = optarg;
			std::vector<std::string> x = split(opt_route_type, ';');
			for (int i = 0; i < x.size(); ++i)
			{
				std::vector<std::string> y = split(x[i], ':');
				RouteInfo rt (i,atoi(y[0].c_str()), atoi(y[1].c_str()), atoi(y[2].c_str()));
				routes_info.push_back(rt);
			}
	        break;
	    }
	    case '?':
	        if (optopt == 's' || optopt == 'r' || optopt == 'i')
            	fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	        else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	        else
                fprintf (stderr,
		                "Unknown option character `\\x%x'.\n",
                        optopt);
            usage();
	        return 1;
	    default:
                usage();
	            abort();
    }

    if (opt_seed == -1)
    {
            fprintf (stderr, "You need to specify option `-s'.\n");
            usage();
            return 1;
    }
    if (opt_route_type.empty())
    {
            fprintf (stderr, "You need to specify option `-r'.\n");
            usage();
            return 1;
    }
    if (opt_instance_prefix.empty())
    {
            fprintf (stderr, "You need to specify option `-i'.\n");
            usage();
            return 1;
    }

    std::cout << "The passed parameters are:" << std::endl;
    for (int i = 0; i < routes_info.size(); ++i)
    {
    	std::cout << "\t" << routes_info[i].tipo_ruta <<  ":";
	std::cout << routes_info[i].quantity << ":" << routes_info[i].min_length << ":" << routes_info[i].max_length << std::endl;
    }


	// PARSE DATA
	DataHandler *dh = new DataHandler();

	std::vector<coordinate_t> coords;
	std::string ip = opt_instance_prefix;
	if (dh->read_coords(ip + "Coords.txt", coords) == -1) abort();
	int **demand;
	if (dh->read_demand(ip + "Demand.txt", demand, size) == -1) abort();
	int **travel_times;
	if (dh->read_travel_times(ip + "TravelTimes.txt", travel_times, size) == -1) abort();

	
	std::vector<BusStop> bus_stops;
	// Initialize BusStops
	int i=0;
	for (std::vector<coordinate_t>::iterator it =  coords.begin(); it!= coords.end(); ++it)
	{
		bus_stops.push_back(BusStop(i, it->x, it->y));
		i++;
    }

	// Initialize problem
	Problem *p = new Problem();
	p->set_name(opt_instance_prefix);
	p->set_size(size);
	p->set_demand(demand);
	p->set_travel_times(travel_times);
	p->set_bus_stops(bus_stops);	

	// Show parsed data (optional)
	if (DEBUG)
	{
		p->show_bus_stops();
		std::cout << "..." << std::endl;
		p->show_demand();
		std::cout << "..." << std::endl;
		p->show_travel_times();	
	}


	// Solution *s = new Solution();
	// Route *r = new Route();
	// Route *r1 = new Route();
	// Route *r2 = new Route();
	// Route *r3 = new Route();
	
	// std::vector<Route> rts;
	// std::vector<BusStop> bs;
	// std::vector<BusStop> bs1;
	// std::vector<BusStop> bs2;
	// std::vector<BusStop> bs3;

	
	// bs.push_back(bus_stops[8]);
	// bs.push_back(bus_stops[14]);

		
	// bs1.push_back(bus_stops[0]);
	// bs1.push_back(bus_stops[1]);
	// bs1.push_back(bus_stops[3]);
	// bs1.push_back(bus_stops[4]);
	
	// bs2.push_back(bus_stops[10]);
	// bs2.push_back(bus_stops[9]);
	// bs2.push_back(bus_stops[6]);
	// bs2.push_back(bus_stops[14]);
	// bs2.push_back(bus_stops[7]);
	// bs2.push_back(bus_stops[5]);
	// bs2.push_back(bus_stops[2]);
	// bs2.push_back(bus_stops[1]);
	
	// bs3.push_back(bus_stops[13]);
	// bs3.push_back(bus_stops[12]);
	// bs3.push_back(bus_stops[10]);
	// bs3.push_back(bus_stops[11]);
	
	// r->set_bus_stops(bs);
	// r1->set_bus_stops(bs1);
	// r2->set_bus_stops(bs2);
	// r3->set_bus_stops(bs3);

	// rts.push_back(r[0]);
	// rts.push_back(r1[0]);
	// rts.push_back(r2[0]);
	// rts.push_back(r3[0]);
	
	// s->set_routes(rts);
	
	// std::cout << "Routes:" << std::endl;
	// s->routes[0].print_route();
	// s->routes[1].print_route();
	// s->routes[2].print_route();
	// s->routes[3].print_route();
	
	// ShortestRoute *sr = new ShortestRoute(size);
	
	// sr->calcDistNoRoutes(travel_times);
	// Route rti;
	// std::cout << "Ruta más corta (0,10):\n";
	// sr->construct_route(0,10,&rti,bus_stops);	
	// for(int i=0; i < rti.bus_stops.size();i++){
	// 	std::cout << rti.bus_stops[i].idi;
	// 	if( i < rti.bus_stops.size() - 1) std::cout << ", ";
	// }
	// std::cout << "\n";


	// sr->calcDist(travel_times,rts);

	// bool y = s->check_connectivity(size);
	// std::cout << "check_connectivity: " << y << std::endl;
	
	// bool x = s->routes[0].check_cycles_and_backtracks();
	// std::cout << "check_cycles_and_backtracks (route: 0): " << x << std::endl;

	// float fo1 = s->setFO1(sr,demand);
	// float fo2 = s->setF02(size,travel_times);

	// s->print_solution_routes();
	
	// std::cout << "FO1: " << fo1 << std::endl;
	// std::cout << "FO2: " << fo2 << std::endl;

	// SolutionSet *st = new SolutionSet();
	// st->solutions.push_back(*s);
	
	// st->print_solution_set();	
 //        float hypervolume = hv(st, p);
	// std::cout<<hypervolume<<std::endl;
	
	Ants *a=new Ants(bus_stops,demand,travel_times,10,1000,opt_seed,routes_info,p,opt_instance_prefix);

	// delete p;
	// delete s;
	// delete r;
	// delete r1;
	// delete r2;
	// delete r3;
	// delete sr;
	
	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < size; ++i)
	{
		delete [] demand[i];
		delete [] travel_times[i];
	}
	delete [] demand;
	delete [] travel_times;

	return 0;
}
