#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <getopt.h>
#include <cstdlib>
#include "BusStop.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "Solution.h"
#include "Route.h"
#include "SolutionSet.h"
#include "Ants.h"
#include "Problem.h"
#include "common.h"
#include "BusStop.h"
#include "RouteInfo.h"
#include "Utils.h"
#include <time.h>


using namespace std;

Ants::Ants(std::vector<BusStop> &bus_stops, int **&demand,int **&travel_times, int nAnts, int nIterations, int good, int bad, int seed,std::vector<RouteInfo> routes_info, Problem *problem,std::string instancia) {
	this->seed=seed;
	srand ( this->seed );
	this->start=time(NULL);
	this->bus_stops = bus_stops;
	this->demand=demand;
	this->problem=problem;
	this->travel_times=travel_times;
	initializePheromoneMatrix(this->bus_stops.size());
	this->nAnts=nAnts;
	this->nIterations=nIterations;
	this->good=good;
	this->bad=bad;
	this->instancia=instancia.substr(instancia.find_last_of("/\\")+1);
	this->minL=routes_info[0].min_length;
	this->maxL=routes_info[0].max_length;
	this->q=routes_info[0].quantity;
	this->calidad1=9999999.9;
	this->calidad2=9999999.9;
	this->st=new SolutionSet();
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

float Ants::getHV(){
	return this->hipV;
}

void Ants::pareto(){
	for (int i = 0; i < this->st->solutions.size(); i++)
	{
		for (int j = 0; j < this->st->solutions.size(); j++)
		{
			if(i!=j)
			{
				if(this->st->solutions[i].fo1 >= this->st->solutions[j].fo1)
				{
					if(this->st->solutions[i].fo2 >= this->st->solutions[j].fo2)
					{
						this->st->solutions.erase(this->st->solutions.begin() + i);
						i--;
						j--;
					}	
				}
			}
		}
	}
	this->setBestSolution(this->st->solutions[this->st->solutions.size()-1].routes);

}

void Ants::getSolutions(){
	std::vector<Route> rts;
	std::vector<BusStop> stops;
	Route r;
	ShortestRoute *sr = new ShortestRoute(size);
	Solution *s = new Solution();


	for (int k = 0; k < this->nIterations; k++)
	{
		for (int i = 0; i < this->nAnts; i++)
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
			int calidad1= s->setFO1(sr,demand);
			int calidad2= s->setF02(this->size,travel_times);
			if(calidad1<=this->calidad1)
			{
				this->calidad1=calidad1;
				if(s->check_connectivity(this->size))
				{
					this->setBestSolution(rts);
					this->st->solutions.push_back(*s);
				}
				for (int j = 0; j < rts.size(); j++)
				{
					this->fillPheromone(rts[j].bus_stops, this->good);
				}
			}
			else{
				for (int j = 0; j < rts.size(); j++)
				{
					this->fillPheromone(rts[j].bus_stops, this->bad);
				}
			}
			if(calidad2<=this->calidad2)
			{
				this->calidad2=calidad2;
				if(s->check_connectivity(this->size))
				{
					this->setBestSolution(rts);
					this->st->solutions.push_back(*s);
				}
				for (int j = 0; j < rts.size(); j++)
				{
					this->fillPheromone(rts[j].bus_stops, 10);
				}
			}
			else{
				for (int j = 0; j < rts.size(); j++)
				{
					this->fillPheromone(rts[j].bus_stops, 1);
				}
			}
		}
	}
	//this->printPheromone();
	this->pareto();
	sr->calcDist(this->travel_times,this->bestSolution);
	s->set_routes(this->bestSolution);
	//for (int i = 0; i < rts.size(); i++)
	//{
	//	this->bestSolution[i].print_route();
		//bool x = s->routes[i].check_cycles_and_backtracks();
	//}	
	//std::cout<<"FO1: "<<s->setFO1(sr,demand)<<" FO2: "<<s->setF02(this->size,travel_times)<<std::endl;
	float hypervolume = hv(this->st, this->problem);
	this->hipV = -hypervolume;
	//std::cout<<"hipervolumen: "<<hypervolume<<std::endl;
	//std::cout<<"cuantas: "<<this->st->solutions.size()<<std::endl;
	s->print_solution_routes();
	bool y = s->check_connectivity(this->size);
	//std::cout << "check_connectivity: " << y << std::endl;
	

	time_t fin = time(NULL);
	ofstream file;
	std::string args ="-i "+this->instancia+" -s "+static_cast<ostringstream*>( &(ostringstream() << this->seed) )->str()+" -r "+static_cast<ostringstream*>( &(ostringstream() << this->q) )->str()+":"+static_cast<ostringstream*>( &(ostringstream() << this->minL) )->str()+":"+static_cast<ostringstream*>( &(ostringstream() << this->maxL) )->str()+" -t "+static_cast<ostringstream*>( &(ostringstream() << this->nIterations) )->str()+" -a "+static_cast<ostringstream*>( &(ostringstream() << this->nAnts) )->str()+" -g "+static_cast<ostringstream*>( &(ostringstream() << this->good) )->str()+" -b "+static_cast<ostringstream*>( &(ostringstream() << this->bad) )->str()+".txt";
	this->st->print_solution_set(args);
	std::string fileName = "./Results/hormigas "+args;
	//std::cout << fileName<<std::endl;
	file.open (fileName.c_str());
	file << "#Rutas: "<<this->q<<"\tLMin: "<<this->minL<<"\tLMax: "<<this->maxL<<"\tSemilla: "<<this->seed<<"\n";
	file << "hipervolumen: "<<hypervolume<<"\tTiempo de Ejecucion: "<<difftime(fin,this->start)<<"\n";
	file << "nIteraciones: "<<this->nIterations<<"\tnAnts: "<<this->nAnts<<"\tg: "<<this->good<<"\tb: "<<this->bad<<"\n\n";
	file <<"-Mejor Solucion\tFO1: "<<s->setFO1(sr,demand)<<"\tFO2: "<<s->setF02(this->size,travel_times)<<"\t Conectividad: "<<y<<"------------------------------------------------------\n";
	for (int i = 0; i < this->bestSolution.size(); i++)
	{
		for (int j = 0; j < this->bestSolution[i].bus_stops.size(); j++)
		{
			file << this->bestSolution[i].bus_stops[j].idi+1;
			if(j < this->bestSolution[i].bus_stops.size()-1){
				file << ", ";
			}
		}
		file <<"\n";
	}
	file << "\n\n";

	for (int i = 0; i < this->st->solutions.size(); ++i)
	{
		file <<"-Solucion "<< i <<"\tFO1: "<<this->st->solutions[i].fo1<<"\tFO2: "<<this->st->solutions[i].fo2<<"\t Conectividad: "<< this->st->solutions[i].check_connectivity(this->size)<<"------------------------------------------------------\n";
		for (int j = 0; j < this->st->solutions[i].routes.size(); j++)
		{
			for (int k = 0; k < this->st->solutions[i].routes[j].bus_stops.size(); k++)
			{
				file << this->st->solutions[i].routes[j].bus_stops[k].idi+1;
				if(k < this->st->solutions[i].routes[j].bus_stops.size()-1){
					file << ", ";
				}
			}
			file << "\n";
		}
		file << "\n\n";
	}
	file.close();
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
