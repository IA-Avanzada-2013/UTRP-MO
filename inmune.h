#ifndef INMUNE_H
#define INMUNE_H

#include "Solution.h"
#include "SolutionSet.h"
#include "Route.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include "opciones.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>

using namespace std;

class Inmune{
	public:
		Inmune(vector<RouteInfo>,Opciones*&,int**&,int**&,vector<BusStop>&,int&,int&);
		Route* generar_ruta_factible(int,vector<BusStop>&,int**&);
		Solution* generar_anticuerpo(vector<BusStop>&,int**&,int);
		void generar_poblacion(SolutionSet&);
		void evaluar_fo(SolutionSet*&);
		bool es_dominado_de_Pareto(Solution, SolutionSet);
		void afinidad(SolutionSet*&);
		void eliminar_dominados(SolutionSet*&);
		vector<float> ordenar_afinidad(SolutionSet*&);
		int get_index(float&, SolutionSet);
		vector<Solution> seleccionar_mejores_anticuerpos(SolutionSet*&);
		void clonar_anticuerpos(vector<Solution>&);
		void mutacion(vector<Solution>&);
		bool valor_in_vector(float,vector<float>);
		void eliminar_exceso(vector<Solution>&);
		void nueva_generacion(SolutionSet*&,vector<Solution>&);

	
		vector<RouteInfo> informacion;
		Opciones opciones;
		int **demand;
		int **travel_times;
		vector<BusStop> bs;
		int size;
		int seed;
		ShortestRoute* sr;
};
#endif
