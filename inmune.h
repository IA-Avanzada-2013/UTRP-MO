#ifndef INMUNE_H
#define INMUNE_H

#include "Solution.h"
#include "SolutionSet.h"
#include "Route.h"
#include "RouteInfo.h"
#include "ShortestRoute.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>

using namespace std;

class Inmune{
	public:
		Inmune(int,vector<RouteInfo>);
		Route* generar_ruta_factible(int,vector<BusStop>&,int**&);
		Solution* generar_anticuerpo(vector<BusStop>&,int**&,int);
		void generar_poblacion(SolutionSet&,int,vector<BusStop>&,int**&);
		void evaluar_fo(SolutionSet*&,int**&,int**&,ShortestRoute*&,int);
		bool es_dominado_de_Pareto(Solution, SolutionSet);
		void afinidad(SolutionSet*&,float,float);
		void eliminar_dominados(SolutionSet*&);
		vector<float> ordenar_afinidad(SolutionSet*&);
		int get_index(float&, SolutionSet);
		vector<Solution> seleccionar_mejores_anticuerpos(SolutionSet*&,float);
		void clonar_anticuerpos(vector<Solution>&,int);
		void mutacion(vector<Solution> &,int,float,vector<BusStop>&,int**&);
		bool valor_in_vector(float,vector<float>);
		void eliminar_exceso(vector<Solution>&,int,float,int**&,int**&,ShortestRoute*&,int,float,float);
		void nueva_generacion(SolutionSet*&,vector<Solution>&,int,int,vector<BusStop>&,int**&,int**&,ShortestRoute*&,int,float,float);

	private:
		int tam_poblacion;
		vector<RouteInfo> informacion;
};
#endif
