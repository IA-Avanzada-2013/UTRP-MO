#include <iostream>
#include <vector>

#include "common.h"
#include "DataHandler.h"
#include "Problem.h"
#include "BusStop.h"
#include "Route.h"


std::string identifiers[] ={ "A","B","C","D","E","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",\
					  "AA","AB","AC","AD","AE","AG","AH","AI","AJ","AK","AL","AM","AN","AO","AP","AQ","AR","AS","AT","AU","AV","AW","AX","AY","AZ", \
					  "BA","BB","BC","BD","BE","BG","BH","BI","BJ","BK","BL","BM","BN","BO","BP","BQ","BR","BS","BT","BU","BV","BW","BX","BY","BZ", \
					  "CA","CB","CC","CD","CE","CG","CH","CI","CJ","CK","CL","CM","CN","CO","CP","CQ","CR","CS","CT","CU","CV","CW","CX","CY","CZ", \
					  "DA","DB","DC","DD","DE","DG","DH","DI","DJ","DK","DL","DM","DN","DO","DP","DQ","DR","DS","DT","DU","DV","DW","DX","DY","DZ"  \
					};

int main(int agrc, char **argv)
{

	int size;
	std::cout << "utrpmo :)" << std::endl;

	// PARSE DATA
	DataHandler *dh = new DataHandler();

	std::vector<coordinate_t> coords;
	dh->read_coords("./instances/MandlCoords.txt", coords);
	int **demand;
	dh->read_demand("./instances/MandlDemand.txt", demand, size);
	int **travel_times;
	dh->read_travel_times("./instances/MandlTravelTimes.txt", travel_times, size);

	
	std::vector<BusStop> bus_stops;
	// Initialize BusStops
	int i=0;
	for (std::vector<coordinate_t>::iterator it =  coords.begin(); it!= coords.end(); ++it)
	{
		bus_stops.push_back(BusStop(identifiers[i], it->x, it->y));
		i++;
    }

	// Initialize problem
	Problem *p = new Problem();

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



	std::vector<BusStop> bs;
	bs.push_back(bus_stops[0]);
	bs.push_back(bus_stops[10]);
	bs.push_back(bus_stops[12]);
	bs.push_back(bus_stops[14]);
	// bs.push_back(bus_stops[14]);
	Route *r = new Route();
	r->set_bus_stops(bs);
	bool x = r->check_cycles_and_backtracks();
	std::cout << "check_cycles_and_backtracks: " << x << std::endl;



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