#ifndef BUS_STOP_H
#define BUS_STOP_H

#include <iostream>
#include "common.h"


class BusStop
{
    
public:
	std::string id;
    float x,y;

	BusStop(std::string id, float x, float y);
    bool operator==(const BusStop& compare);

};


#endif