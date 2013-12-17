#include "BusStop.h"

BusStop::BusStop(std::string id, float x, float y, int idi) {
	this->id = id;
	this->x = x;
	this->y = y;
	this->idi=idi;
};

bool BusStop::operator==(const BusStop& compare) 
{ 
	return compare.id == this->id; 
}
