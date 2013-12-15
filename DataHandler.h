#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "common.h"



class DataHandler
{
  public:

	DataHandler(void);
	~DataHandler(void);
	
	int read_coords(std::string filename, std::vector<coordinate_t>& result);
	int read_demand(std::string filename, int **&result, int &size);
	int read_travel_times(std::string filename, int **&result, int &size);
	int read_route_type(std::string filename, struct Ivan *result);
};

#endif
