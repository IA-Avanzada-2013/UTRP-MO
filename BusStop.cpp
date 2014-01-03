#include "BusStop.h"

BusStop::BusStop(int idi, float x, float y) {
	this->x = x;
	this->y = y;
	this->idi=idi;
};

bool BusStop::operator==(const BusStop& compare) 
{ 
	return compare.idi == this->idi; 
}
