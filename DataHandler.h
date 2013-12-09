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
	
	errno_t read_coords(std::string filename, std::vector<coordinate_t>& result);

	errno_t read_demand(std::string filename, struct Ivan *result);
	errno_t read_travel_times(std::string filename, struct Ivan *result);
	errno_t read_route_type(std::string filename, struct Ivan *result);
};

#endif