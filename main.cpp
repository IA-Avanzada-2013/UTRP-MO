#include <iostream>
#include <vector>

#include "common.h"
#include "DataHandler.h"


int main(int agrc, char **argv)
{

	std::cout << "utrpmo :)" << std::endl;

	DataHandler *dh = new DataHandler();

	std::string filename = "./instances/MandlCoords.txt";
	
	std::vector<coordinate_t> coords;
	dh->read_coords(filename, coords);

	// for (std::vector<coordinate_t>::iterator it =  coords.begin(); it!= coords.end(); ++it) {
	//     std::cout << it->x << std::endl;
	// }

	return 0;
}