#include <iostream>
#include <vector>

#include <getopt.h>
#include <cstdlib>
#include <ctime>

#include "common.h"
#include "DataHandler.h"
#include "Problem.h"
#include "Solution.h"
#include "BusStop.h"
#include "Route.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "SolutionSet.h"
#include "Utils.h"
#include "inmune.h"
#include "opciones.h"

#define POP_SIZE 1000
#define ALPHA 0.4
#define BETA 0.6
#define CLON_SIZE 150
#define PROBMUTACION 0.1
#define AFINIDAD 1
#define GENERACIONES 10
#define CLONES 0.5
#define REEMPLAZO 0.3


using namespace std;

void intro(void)
{
	std::string s = 	"____ __________________________________  _____   ________   		\n"
						"|    |   \\__    ___/\\______   \\______   \\/     \\  \\_____  \\ \n"
						"|    |   / |    |    |       _/|     ___/  \\ /  \\  /   |   \\ 	\n"
						"|    |  /  |    |    |    |   \\|    |  /    Y    \\/    |    \\	\n"
						"|______/   |____|    |____|_  /|____|  \\____|__  /\\_______  /	\n"
						"                            \\/                 \\/         \\/ 	\n";

	s += ">>> UTRP Multi Objective C++ Approach by UTFSM Students :D <<< ";
	std::cout << s << std::endl;
}

void usage(void)
{
	std::string s = 	"Usage: ./utrpmo -i INSTANCE_PREFIX -s SEED -r ROUTE_INFO\n"
						"For example: ./utrpmo -i instances/Mandl -s 12345 -r \"10:2:7;12:3:8\" \n";

	std::cout << s << std::endl;
}

int main(int argc, char **argv)
{
	time_t inicio;
	time_t fin;
	double segundos;
	time(&inicio);
	
	intro();

	int size;
    char c;

    int opt_seed = -1;
    std::string opt_route_type;
    std::string opt_instance_prefix;
    std::vector<RouteInfo> routes_info;


    while ((c = getopt(argc, argv, "i:s:r:")) != -1)
    switch (c)
    {
    	case 'i':
    		opt_instance_prefix = optarg;
    		break;
	    case 's':
	        opt_seed = atoi(optarg);
	        break;
   	    case 'r':
   	    {
	        opt_route_type = optarg;
			std::vector<std::string> x = split(opt_route_type, ';');
			for (int i = 0; i < x.size(); ++i)
			{
				std::vector<std::string> y = split(x[i], ':');
				RouteInfo rt (i,atoi(y[0].c_str()), atoi(y[1].c_str()), atoi(y[2].c_str()));
				routes_info.push_back(rt);
			}
	        break;
	    }
	    case '?':
	        if (optopt == 's' || optopt == 'r' || optopt == 'i')
            	fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	        else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	        else
                fprintf (stderr,
		                "Unknown option character `\\x%x'.\n",
                        optopt);
            usage();
	        return 1;
	    default:
                usage();
	            abort();
    }

    if (opt_seed == -1)
    {
            fprintf (stderr, "You need to specify option `-s'.\n");
            usage();
            return 1;
    }
    if (opt_route_type.empty())
    {
            fprintf (stderr, "You need to specify option `-r'.\n");
            usage();
            return 1;
    }
    if (opt_instance_prefix.empty())
    {
            fprintf (stderr, "You need to specify option `-i'.\n");
            usage();
            return 1;
    }

    std::cout << "The passed parameters are:" << std::endl;
    for (int i = 0; i < routes_info.size(); ++i)
    {
    	std::cout << "\t" << routes_info[i].tipo_ruta <<  ":";
	std::cout << routes_info[i].quantity << ":" << routes_info[i].min_length << ":" << routes_info[i].max_length << std::endl;
    }


	// PARSE DATA
	DataHandler *dh = new DataHandler();

	std::vector<coordinate_t> coords;
	std::string ip = opt_instance_prefix;
	if (dh->read_coords(ip + "Coords.txt", coords) == -1) abort();
	int **demand;
	if (dh->read_demand(ip + "Demand.txt", demand, size) == -1) abort();
	int **travel_times;
	if (dh->read_travel_times(ip + "TravelTimes.txt", travel_times, size) == -1) abort();

	
	std::vector<BusStop> bus_stops;
	// Initialize BusStops
	int i=0;
	for (std::vector<coordinate_t>::iterator it =  coords.begin(); it!= coords.end(); ++it)
	{
		bus_stops.push_back(BusStop(i, it->x, it->y));
		i++;
    }

	// Initialize problem
	Problem *p = new Problem();
	p->set_name(opt_instance_prefix);
	p->set_size(size);
	p->set_demand(demand);
	p->set_travel_times(travel_times);
	p->set_bus_stops(bus_stops);	

	// Show parsed data (optional)
	if (DEBUG)
	{
		p->show_bus_stops();
		std::cout << "..." << std::endl;
		p->show_demand();
		std::cout << "..." << std::endl;
		p->show_travel_times();	
	}
		
	//parametros del algoritmo
	Opciones* o = new Opciones();
	
	o->set_popsize(100);
	o->set_alpha(0.5);
	o->set_beta(0.5);
	o->set_clonsize(150);
	o->set_probmutacion(0.1);
	o->set_afinidad(1);
	o->set_generaciones(10);
	o->set_porcentajeclones(0.5);
	o->set_porcentajereemplazo(0.3);

	//inicializacion del algoritmo
	Inmune *algoritmo = new Inmune(routes_info,o);
	
	//inicializacion de la poblacionvector bad_alloc c++
	SolutionSet *poblacion = new SolutionSet();
	
	vector<Solution> sol;
	poblacion->solutions=sol;
	
	//se genera la poblacion aleatoriamente
	algoritmo->generar_poblacion(*poblacion,POP_SIZE,bus_stops,travel_times);
	
	//mostrar la poblacion
	for(int i=0;i<POP_SIZE;i++)
	{
		cout << "Solucion " << i << endl;
		for(int j=0;j<poblacion->solutions[i].routes.size();j++)
		{
			poblacion->solutions[i].routes[j].print_route();
		}
		cout << endl;
	}
	
	ShortestRoute *sr = new ShortestRoute(size);
	sr->calcDistNoRoutes(travel_times);
	
	//empieza el ciclo
	
	int generacion = 1;
	
	while(generacion<GENERACIONES)
	{
		
		//evaluacion de las funciones objetivo de las soluciones
		algoritmo->evaluar_fo(poblacion,travel_times,demand,sr,size);
		
		//calculo de afinidad
		algoritmo->afinidad(poblacion,ALPHA,BETA);
		
		//se consideran solo las soluciones no dominadas del problema
		algoritmo->eliminar_dominados(poblacion);
				
		//seleccion de los mejores individuos
		vector<Solution> clones = algoritmo->seleccionar_mejores_anticuerpos(poblacion,AFINIDAD);
		
		//seleccion clonal
		algoritmo->clonar_anticuerpos(clones,CLON_SIZE);
		
		//mutacion
		algoritmo->mutacion(clones,opt_seed,PROBMUTACION,bus_stops,travel_times);
		
		//se elimina el exceso de anticuerpos
		algoritmo->eliminar_exceso(clones,POP_SIZE,CLONES,travel_times,demand,sr,size,ALPHA,BETA);
		
		//se incorporan nuevos anticuerpos a la nueva generacion
		algoritmo->nueva_generacion(poblacion,clones,POP_SIZE,REEMPLAZO,bus_stops,travel_times,demand,sr,size,ALPHA,BETA);
		
		cout << poblacion->solutions.size() << endl;
		
		//se guardan los optimos de pareto para la generacion
		stringstream sstr;
		sstr << "results/pareto_" << generacion << ".txt";
		string nombre = sstr.str();
		
		ofstream archivo(nombre.c_str());
		
		for(int i=1;i<poblacion->solutions.size();i++)
		{
			if(!algoritmo->es_dominado_de_Pareto(poblacion->solutions[i],*poblacion))
			{
				archivo << poblacion->solutions[i].fo1 << " " << poblacion->solutions[i].fo2 << endl;
			}
		}
		archivo.close();
		
		
		//se aumenta en uno la generacion
		generacion++;
		
		//se repite el proceso hasta cumplir con la condicion de termino
	}
	
	//st->print_solution_set();	
    float hypervolume = hv(poblacion, p);
	std::cout<<hypervolume<<std::endl;
	
	/*
	
	
	for(int i=0;i<poblacion->solutions.size();i++)
	{
		cout << i << " es dominado " << algoritmo->es_dominado_de_Pareto(poblacion->solutions[i],*poblacion) << endl;
		cout << poblacion->solutions[i].fo1 << " " << poblacion->solutions[i].fo2 << endl;
		cout << poblacion->solutions[i].quality << endl;
	}
	
	
	*/
	
	/*
	Solution *s = algoritmo->generar_anticuerpo(bus_stops,travel_times,1245);
	
	
	for(unsigned int i=0;i<s->routes.size();i++)
	{
		s->routes[i].print_route();
	}
	
	Route rti;
	std::cout << "Ruta más corta (0,10):\n";
	sr->construct_route(0,10,&rti,bus_stops);	
	for(int i=0; i < rti.bus_stops.size();i++){
		std::cout << rti.bus_stops[i].idi;
		if( i < rti.bus_stops.size() - 1) std::cout << ", ";
	}
	std::cout << "\n";


	//sr->calcDist(travel_times,rts);

	bool y = s->check_connectivity(size);
	std::cout << "check_connectivity: " << y << std::endl;
	
	bool x;
	
	for (int i=0;i<s->routes.size();i++)
	{
		x = s->routes[i].check_cycles_and_backtracks();
		std::cout << "check_cycles_and_backtracks (route: " << i << " ):" << x << std::endl;
	}
	
	float fo1 = s->setFO1(sr,demand);
	float fo2 = s->setF02(size,travel_times);

	s->print_solution_routes();
	
	std::cout << "FO1: " << fo1 << std::endl;
	std::cout << "FO2: " << fo2 << std::endl;
	*/
	//SolutionSet *st = new SolutionSet();
	//st->solutions.push_back(*s);
	
	//st->print_solution_set();	
    //    float hypervolume = hv(st, p);
	//std::cout<<hypervolume<<std::endl;


	delete p;
	//delete s;
	//delete r;
	//delete r1;
	//delete r2;
	//delete r3;
	delete sr;
	
	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < size; ++i)
	{
		delete [] demand[i];
		delete [] travel_times[i];
	}
	delete [] demand;
	delete [] travel_times;

	time(&fin);
	segundos = difftime(fin,inicio);
	
	cout << segundos << " segundos transcurridos" << endl;
	
	return 0;
}
