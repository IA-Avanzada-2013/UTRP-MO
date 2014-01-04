#include "DataHandler.h"

#include <cstdlib>

DataHandler::DataHandler(void)
{

}

DataHandler::~DataHandler(void)
{

}

int DataHandler::read_coords(std::string filename, std::vector<coordinate_t>& result)
{
	std::string line;
	std::ifstream myfile(filename.c_str());

	coordinate_t aux_coord;

	if (myfile.is_open())
	{
		getline(myfile,line); // first line not needed, contains size which is handled by vector
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
		return -1;
	}
	
	return 0;
}

int DataHandler::read_demand(std::string filename, int **&result, int &rsize)
{
	std::string line;
	std::ifstream myfile(filename.c_str());

	int size = 0;
	int row = 0;

	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			getline(myfile,line);
			if (line.empty() || line.size()<=1) // second condition is a dirty fix .. but works :P
			{ 
				// std::cout << "skipping empty line" << std::endl;
				continue; 
			}
			std::vector<std::string> x = split(line, ' ');

			if(size == 0) // first run, we need to allocate memory
			{
				
				rsize = size = x.size();
				// Allocate memory
				result = new int*[size];
				for (int i = 0; i < size; ++i)
					result[i] = new int[size];
			}
			for (int col = 0; col < size; col++)
			{
				// trim trailing spaces
				
				size_t endpos = x[col].find_last_not_of("\r\n");
				if( std::string::npos != endpos )
				{
				    x[col] = x[col].substr( 0, endpos+1 );
				}

				// std::cout << "assigning to row " << row << " column " << col << " value " << x[col] << " compare: " << x[col].compare("Inf") << std::endl;	
				if(x[col].compare("Inf") == 0) x[col] = "-1";
				result[row][col] = ::atoi(x[col].c_str());
			}
			row++;
		}
		myfile.close();
	}
	else
	{ 
		std::cout << "Unable to open file";
		return -1;
	}
	
	return 0;
}
int DataHandler::read_travel_times(std::string filename, int **&result, int &rsize)
{
	return DataHandler::read_demand(filename, result, rsize);
}

int DataHandler::read_route_type(std::string filename, struct Ivan *result)
{
	return 0;
}
