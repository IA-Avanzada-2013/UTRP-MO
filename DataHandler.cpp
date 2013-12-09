#include "DataHandler.h"

DataHandler::DataHandler(void)
{

}

DataHandler::~DataHandler(void)
{

}

errno_t DataHandler::read_coords(std::string filename, std::vector<coordinate_t>& result)
{
	std::string line;
	std::ifstream myfile(filename);

	coordinate_t aux_coord;

	if (myfile.is_open())
	{
		getline(myfile,line); // first line not needed
		while (!myfile.eof())
		{
		  getline(myfile,line);
		  if (line.empty()) continue;
		  std::vector<std::string> x = split(line, ' ');
		  aux_coord.x = ::atof(x[0].c_str());
		  aux_coord.y = ::atof(x[1].c_str());
		  result.push_back(aux_coord);
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