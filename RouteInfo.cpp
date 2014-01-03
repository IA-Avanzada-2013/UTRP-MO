#include "RouteInfo.h"

RouteInfo::RouteInfo(int tr, int q, int minl, int maxl)
{
	this->tipo_ruta = tr;
	this->quantity = q;
	this->min_length = minl;
	this->max_length = maxl;
}
