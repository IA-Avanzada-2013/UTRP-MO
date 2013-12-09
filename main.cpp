#include <iostream>
#include "common.h"
#include "DataHandler.h"


int main(int agrc, char **argv)
{

	std::cout << "utrpmo :)" << std::endl;
	
	coordinate_t *coords;

	DataHandler *dh = new DataHandler();

	std::string filename = "./instances/MandlCoords.txt";
	dh->read_coords(filename, NULL);

	return 0;
}