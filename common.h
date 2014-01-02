#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <ctype.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <list>
#include <getopt.h>
#else
#endif

#define DEBUG 1

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

void show_matrix(int **&matrix, int size);

#endif
