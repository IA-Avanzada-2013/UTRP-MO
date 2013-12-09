#include "DataHandler.h"

DataHandler::DataHandler(void)
{

}

DataHandler::~DataHandler(void)
{

}

errno_t DataHandler::read_coords(std::string filename, struct Ivan *result)
{
	std::string line;
	std::ifstream myfile(filename);
	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
		  getline(myfile,line);
		  std::cout << line << std::endl;
		}
		myfile.close();
	}
	else
	{ 
		std::cout << "Unable to open file";
	}
	
	return 0;
}

errno_t DataHandler::read_demand(std::string filename, struct Ivan *result)
{
	return 0;
}
errno_t DataHandler::read_travel_times(std::string filename, struct Ivan *result)
{
	return 0;
}
errno_t DataHandler::read_route_type(std::string filename, struct Ivan *result)
{
	return 0;
}