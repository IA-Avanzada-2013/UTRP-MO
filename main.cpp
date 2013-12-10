#include <iostream>
#include <vector>

#include "common.h"
#include "DataHandler.h"
#include "Problem.h"


int main(int agrc, char **argv)
{

	int size;
	std::cout << "utrpmo :)" << std::endl;

	DataHandler *dh = new DataHandler();
	
	std::vector<coordinate_t> coords;
	dh->read_coords("./instances/MandlCoords.txt", coords);

	int **demand;
	dh->read_demand("./instances/MandlDemand.txt", demand, size);


	int **travel_times;
	dh->read_travel_times("./instances/MandlTravelTimes.txt", travel_times, size);


	Problem *p = new Problem();

	p->set_size(size);
	p->set_demand(demand);
	p->set_travel_times(travel_times);

	
	p->show_demand();
	std::cout << "..." << std::endl;
	p->show_travel_times();


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