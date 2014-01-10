#ifndef ROUTE_INFO_H
#define ROUTE_INFO_H

class RouteInfo
{

public:
	RouteInfo(int tr, int q, int minl, int maxl);
	~RouteInfo(void);
	int tipo_ruta;
	int quantity;
	int max_length;
	int min_length;
	
};

#endif
