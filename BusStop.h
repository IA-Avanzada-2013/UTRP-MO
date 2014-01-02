#ifndef BUS_STOP_H
#define BUS_STOP_H

#include <iostream>
#include "common.h"


class BusStop
{
    
public:
    int idi;
    float x,y;

    BusStop(int idi, float x, float y);
    bool operator==(const BusStop& compare);

};


#endif
