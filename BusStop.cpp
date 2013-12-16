#include "BusStop.h"

BusStop::BusStop(std::string id, float x, float y) {
	this->id = id;
	this->x = x;
	this->y = y;
};

bool BusStop::operator==(const BusStop& compare) 
{ 
	return compare.id == this->id; 
}
