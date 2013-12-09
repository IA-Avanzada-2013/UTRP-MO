#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <ctype.h>


#define COORDS_FILENAME "coords.txt"
#define DEMAND_FILENAME "demand.txt"
#define TRAVEL_FILENAME "travel.txt"
#define ROUTE_TYPE_FILENAME "route_types.txt"

struct Ivan { char i;};

typedef struct _coordinate {
    float x;    
    float y;
} coordinate_t;


// http://stackoverflow.com/a/236803
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);


#endif