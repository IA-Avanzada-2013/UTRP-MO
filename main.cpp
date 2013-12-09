#include <iostream>
#include <vector>

#include "common.h"
#include "DataHandler.h"


int main(int agrc, char **argv)
{

	int size;
	std::cout << "utrpmo :)" << std::endl;

	DataHandler *dh = new DataHandler();
	
	std::vector<coordinate_t> coords;
	dh->read_coords("./instances/MandlCoords.txt", coords);

	int **demand;
	dh->read_demand("./instances/MandlDemand.txt", demand, size);

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			std::cout << demand[i][j] << " ";
		}
		std::cout << std::endl;
	}

	int **travel_times;
	dh->read_travel_times("./instances/MandlTravelTimes.txt", travel_times, size);
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			std::cout << travel_times[i][j] << " ";
		}
		std::cout << std::endl;
	}


	// for (std::vector<coordinate_t>::iterator it =  coords.begin(); it!= coords.end(); ++it) {
	//     std::cout << it->x << std::endl;
	// }

	return 0;
}