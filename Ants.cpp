#include <iostream>
#include <vector>
#include <algorithm>

#include <getopt.h>
#include <cstdlib>
#include "BusStop.h"

#include "Ants.h"

Ants::Ants(std::vector<BusStop> &bus_stops, int **&demand, int nAnts, int nIterations) {
	this->bus_stops = bus_stops;
	this->demand=demand;
	initializePheromoneMatrix(this->bus_stops.size());
	this->nAnts=nAnts;
	this->nIterations=nIterations;
	for (int i = 0; i < nAnts; i++)
	{
		this->antMakeRoute();
	}
	this->printPheromone();
};


void Ants::initializePheromoneMatrix(int size){
	this->size=size;
	std::vector<int> temp(size,1); 
	std::vector<std::vector<int> > feromonas(size,temp); 
	this->feromonas=feromonas;
	//this->printPheromone();
	//this->printBusStops(this->bus_stops);
	//this->printPheromone();
}

void Ants::printBusStops(std::vector<BusStop> bus_stops){
	for (int i = 0; i < bus_stops.size(); i++)
	{
		std::cout << std::setw(3)<< bus_stops[i].idi <<"\t"<< bus_stops[i].x <<"\t"<< bus_stops[i].y << std::endl;
	}
		std::cout << std::endl;
}

void Ants::printPheromone(){
	std::cout<<"  ";
	for (int j = 0; j < this->size; j++)
	{
		std::cout << std::setw(4) << this->bus_stops[j].idi<<" ";
	}
	std::cout << std::endl;
	for (int i = 0; i < this->size; i++)
	{
		std::cout<< this->bus_stops[i].idi <<" ";
		for (int j = 0; j < this->size; j++)
		{
			std::cout << std::setw(4) << this->feromonas[i][j] <<" ";
		}
		std::cout << std::endl;
	}
}

std::vector<BusStop> Ants::antMakeRoute(){
	std::vector<BusStop> stops=this->bus_stops;
   	srand ( time(NULL) );
   	std::vector<BusStop> bs;
   	int p=this->verticalRoulette();

   	do{
   		bs.push_back(stops[p]);
   		int asd = this->getPositionInBusStops(stops[p]);
   		stops.erase(stops.begin()+p);
   		if(stops.size()>0)
   		{
   			p=this->horizontalRoulette(stops,p);
   		}
   		else{
   			p=0;
   		}
   	}while(stops.size()>0);
   	//this->printBusStops(bs);
   	this->fillPheromone(bs);
	return bs;
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
	srand ( time(NULL) );
	for (int i = 0; i < this->bus_stops.size(); i++)
	{
		sum+=this->feromonas[0][i];
	}

	int number = rand() % sum;
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
	srand ( time(NULL) );
	for (int i = 0; i < stops.size(); i++)
	{
		sum+=this->feromonas[this->getPositionInBusStops(stops[i])][p];
	}
	int number = rand() % sum;
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

void Ants::fillPheromone(std::vector<BusStop> stops){
	BusStop bs = stops[0];
	int pos=this->getPositionInBusStops(bs);
	feromonas[pos][pos]++;
	for (int i = 1; i < stops.size(); ++i)
	{
		feromonas[this->getPositionInBusStops(stops[i])][pos]++;
		pos=this->getPositionInBusStops(stops[i]);
	}
	//this->printPheromone();
}
