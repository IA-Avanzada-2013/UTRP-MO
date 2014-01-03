#ifndef ROUTE_INFO_H
#define ROUTE_INFO_H

class RouteInfo
{
public:
	int tipo_ruta;
	int quantity;
	int max_length;
	int min_length;
	RouteInfo(int tr, int q, int minl, int maxl);
};

#endif
