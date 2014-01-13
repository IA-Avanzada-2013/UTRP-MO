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

Ants::Ants(std::vector<BusStop> &bus_stops, int **&demand,int **&travel_times, int nAnts, int nIterations,int seed,std::vector<RouteInfo> routes_info) {
	this->seed=seed;
	srand ( this->seed );
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
};


void Ants::initializePheromoneMatrix(int size){
	this->size=size;
	std::vector<double> temp(size,1); 
	std::vector<std::vector<double> > feromonas(size,temp); 
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

std::vector<BusStop> Ants::cleanDestinos(std::vector<BusStop> bs, std::vector<BusStop> stops){
	for (int i = 0; i < bs.size(); i++)
	{
		stops.erase(std::remove(stops.begin(), stops.end(), bs[i]), stops.end());
	}

	return stops;
}


std::vector<BusStop> Ants::antMakeRoute(){
	std::vector<BusStop> stops=this->bus_stops;
   	std::vector<BusStop> bs;
   	int p=this->verticalRoulette();
   	BusStop B=this->bus_stops[p];
   	bool flag =true;
   	int i =0;
   	do{
   		B=stops[p];
   		bs.push_back(B);
   		stops=this->getPosiblesDestinos(B);
   		stops=this->cleanDestinos(bs,stops);
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
	return bs;
}

int Ants::random(int max){
	if(max==0)
	{
		return 0;
	}
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
		rts.clear();
		while(rts.size()<this->q)
		{
			stops=this->antMakeRoute();
			r.set_bus_stops(stops);
			rts.push_back(r);
			this->actualSolution=rts;
		}
		sr->calcDist(this->travel_times,rts);
		s->set_routes(rts);
		int calidad= s->setFO1(sr,demand) + s->setF02(this->size,travel_times);
		if(calidad<=this->calidad)
		{
			this->calidad=calidad;
			this->setBestSolution(rts);
			for (int i = 0; i < rts.size(); i++)
			{
				this->fillPheromone(rts[i].bus_stops, 10);
			}
		}
		else{
			for (int i = 0; i < rts.size(); i++)
			{
				this->fillPheromone(rts[i].bus_stops, 1);
			}
		}
	}
	this->printPheromone();
	sr->calcDist(this->travel_times,this->bestSolution);
	s->set_routes(this->bestSolution);
	for (int i = 0; i < rts.size(); i++)
	{
		this->bestSolution[i].print_route();
		//bool x = s->routes[i].check_cycles_and_backtracks();
	}	
	std::cout<<"FO1: "<<s->setFO1(sr,demand)<<" FO2: "<<s->setF02(this->size,travel_times)<<std::endl;
	s->print_solution_routes();
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
	int posicion=0;
	std::vector<int> solo(this->size,0);
	for (int i = 0; i < this->actualSolution.size(); i++)
	{
		for (int j = 0; j < this->actualSolution[i].bus_stops.size(); j++)
			{
				solo[this->getPositionInBusStops(this->actualSolution[i].bus_stops[j])]++;
			}	
	}
	for (int i = 0; i < solo.size() && solo[i]!=0; i++)
	{
		if(solo[i]==0)
		{
			posicion=i;
		}
	}
	if(posicion==0)
	{
		posicion=this->random(this->size);
	}
	return posicion;
}

std::vector<BusStop> Ants::getPosiblesDestinos(BusStop bs){
	int posicion=this->getPositionInBusStops(bs);
	std::vector<BusStop> stops;
	for (int i = 0; i < this->size; i++)
	{
		if(this->travel_times[posicion][i]>0)
		{
			stops.push_back(this->bus_stops[i]);
		}
	}
	return stops;
}

int Ants::horizontalRoulette(std::vector<BusStop> stops,int p){
	float sum=0.0;
	for (int i = 0; i < stops.size(); i++)
	{
		sum+=this->feromonas[this->getPositionInBusStops(stops[i])][p];
		
	}
	int number = random(sum);
	int posicion=0;
	sum=0.0;
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
	int cant=0;
	for (int i = 1; i < stops.size(); ++i)
	{
		cant=feromonas[this->getPositionInBusStops(stops[i])][pos]+cantidad;
		if(stops.size()<this->minL)
		{
			cant=cant/2;
		}
		feromonas[this->getPositionInBusStops(stops[i])][pos]=cant;
		pos=this->getPositionInBusStops(stops[i]);
	}
}
