#ifndef BUS_STOP_H
#define BUS_STOP_H

#include <iostream>
#include "common.h"


class BusStop
{
    
public:
	std::string id;
	int idi;
    float x,y;

	BusStop(std::string id, float x, float y, int idi);
    bool operator==(const BusStop& compare);

};


#endif
