#include <iostream>
#include <vector>
#include <algorithm>

#include <getopt.h>
#include <cstdlib>
#include "BusStop.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "Solution.h"
#include "Route.h"

#include "Ants.h"

Ants::Ants(std::vector<BusStop> &bus_stops, int **&demand,int **&travel_times, int nAnts, int nIterations,std::vector<RouteInfo> routes_info) {
	this->bus_stops = bus_stops;
	this->demand=demand;
	this->travel_times=travel_times;
	initializePheromoneMatrix(this->bus_stops.size());
	this->nAnts=nAnts;
	this->nIterations=nIterations;
	this->minL=routes_info[0].min_length;
	this->maxL=routes_info[0].max_length;
	this->q=routes_info[0].quantity;
	this->calidad=9999999.9;
	this->getSolutions();
	srand ( time(NULL) );
};


void Ants::initializePheromoneMatrix(int size){
	this->size=size;
	std::vector<int> temp(size,1); 
	std::vector<std::vector<int> > feromonas(size,temp); 
	this->feromonas=feromonas;
}

void Ants::printBusStops(std::vector<BusStop> bus_stops){
	for (int i = 0; i < bus_stops.size(); i++)
	{
		std::cout << std::setw(3)<< bus_stops[i].idi <<"\t"<< bus_stops[i].x <<"\t"<< bus_stops[i].y << std::endl;
	}
		std::cout << std::endl;
}

void Ants::printPheromone(){
	std::cout<<"\t";
	for (int j = 0; j < this->size; j++)
	{
		std::cout << std::setw(4) << this->bus_stops[j].idi<<" ";
	}
	std::cout << std::endl;
	for (int i = 0; i < this->size; i++)
	{
		std::cout<< this->bus_stops[i].idi <<"\t";
		for (int j = 0; j < this->size; j++)
		{
			std::cout << std::setw(4) << this->feromonas[i][j] <<" ";
		}
		std::cout << std::endl;
	}
}

std::vector<BusStop> Ants::antMakeRoute(){
	std::vector<BusStop> stops=this->bus_stops;
   	std::vector<BusStop> bs;
   	int p=this->verticalRoulette();
   	bool flag =true;
   	int i =0;
   	do{
   		bs.push_back(stops[p]);
   		int asd = this->getPositionInBusStops(stops[p]);
   		stops.erase(stops.begin()+p);
   		i++;
   		if(bs.size()>=this->minL)
   		{
   			if(random(5)==1)
   			{
   				flag=false;
   			}
   		}
   		if(bs.size()>=this->maxL)
   		{
   			flag=false;
   		}
   		if(stops.size()>0)
   		{
   			p=this->horizontalRoulette(stops,p);
   		}
   		else{
   			p=0;
   		}
   	}while(flag && stops.size()>0 );
   	//this->printBusStops(bs);
	return bs;
}

int Ants::random(int max){
	return rand() % max;

}

void Ants::setBestSolution(std::vector<Route> rts){
	this->bestSolution=rts;
	
}

void Ants::getSolutions(){
	std::vector<Route> rts;
	std::vector<BusStop> stops;
	Route r;
	ShortestRoute *sr = new ShortestRoute(size);
	Solution *s = new Solution();


	for (int i = 0; i < this->nIterations; i++)
	{
		while(rts.size()<this->q)
		{
			stops=this->antMakeRoute();
			r.set_bus_stops(stops);
			rts.push_back(r);

		}
		sr->calcDist(this->travel_times,rts);
		s->set_routes(rts);
		int calidad= s->setFO1(sr,demand) + s->setF02(this->size,travel_times);
		if(calidad<=this->calidad)
		{
			this->setBestSolution(rts);
			for (int i = 0; i < rts.size(); i++)
			{
				this->fillPheromone(rts[i].bus_stops, 3);
			}
		}
		else{
			this->fillPheromone(rts[i].bus_stops, 1);
		}
	}
	this->printPheromone();
	for (int i = 0; i < rts.size(); i++)
	{
		//this->printBusStops(rts[i].bus_stops);
		this->bestSolution[i].print_route();
		bool x = s->routes[i].check_cycles_and_backtracks();
		std::cout << "check_cycles_and_backtracks: " << x << std::endl;
	}	
	bool y = s->check_connectivity(this->size);
	std::cout << "check_connectivity: " << y << std::endl;
}

int Ants::getPositionInBusStops(BusStop bus){
	int pos=0;
	for (int i = 0;  i < this->bus_stops.size()  ; i++)
	{
		if(bus.idi==this->bus_stops[i].idi)
		{
			pos=i;
		}
	}
	return pos;

}

int Ants::verticalRoulette(){
	int sum=0;
	for (int i = 0; i < this->bus_stops.size(); i++)
	{
		sum+=this->feromonas[0][i];
	}

	int number = random(sum);
	int posicion=0;
	sum=0;
	for (int i = 0; (i < this->bus_stops.size()) || (sum>number); i++)
	{
		if(sum<=number)
		{
			posicion=i;
		}
		sum+=this->feromonas[0][i];
	}

	return posicion;
}

int Ants::horizontalRoulette(std::vector<BusStop> stops,int p){
	int sum=0;
	for (int i = 0; i < stops.size(); i++)
	{
		sum+=this->feromonas[this->getPositionInBusStops(stops[i])][p];
	}
	int number = random(sum);
	int posicion=0;
	sum=0;
	for (int i = 0; i < stops.size(); i++)
	{
		if(sum<=number)
		{
			posicion=i;
		}
		sum+=this->feromonas[this->getPositionInBusStops(stops[i])][p];
	}
	return posicion;
}

void Ants::fillPheromone(std::vector<BusStop> stops, int cantidad){
	BusStop bs = stops[0];
	int pos=this->getPositionInBusStops(bs);
	feromonas[pos][pos]++;
	for (int i = 1; i < stops.size(); ++i)
	{
		feromonas[this->getPositionInBusStops(stops[i])][pos]=feromonas[this->getPositionInBusStops(stops[i])][pos]+cantidad;
		pos=this->getPositionInBusStops(stops[i]);
	}
	//this->printPheromone();
}
