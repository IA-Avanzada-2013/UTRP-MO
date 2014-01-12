#include "inmune.h"

using namespace std;

Inmune::Inmune(vector<RouteInfo> info,Opciones* &opc)
{
	this->informacion = info;
	this->opciones = opc;
}	

Route* Inmune::generar_ruta_factible(int tipo,vector<BusStop> &bus_stops,int** &travel_times)
{
	int cantidad=0;
	Route *r;
	while(cantidad<1)
	{
		Route *r = new Route();
		std::vector<BusStop> bs;
		
		//se define un largo aleatorio entre el minimo y el maximo ingresado
		int aleatorio = informacion[tipo].min_length+rand()%(informacion[tipo].max_length-informacion[tipo].min_length);
		//generacion de rutas iniciales
		for(int j=0;j<aleatorio;j++)
		{
			int stop_random=rand()%bus_stops.size();
			if(j!=0)
			{
				//verifica que la ruta sea conexa
				while(travel_times[bs[j-1].idi][stop_random]==-1)
				{
					stop_random=rand()%bus_stops.size();
				}
			}
			bs.push_back(bus_stops[stop_random]);
		}
		r->set_bus_stops(bs);
		r->define_bus_type(informacion[tipo].tipo_ruta);
		
		//solo se consideran rutas que no tengan ciclos ni backtracks
		if(!r->check_cycles_and_backtracks())
		{
			cantidad++;
		}
		delete r;
		
	}
	return r;
}

Solution* Inmune::generar_anticuerpo(vector<BusStop> &bus_stops,int** &travel_times,int seed)
{
	//inicializacion de la solucion
	srand(seed);
	Solution *s = new Solution();
	std::vector<Route> rts;
	
	for(unsigned int i=0;i<this->informacion.size();i++)
	{
		int cantidad=0;
		while(cantidad<this->informacion[i].quantity)
		{
			Route *r = new Route();
			std::vector<BusStop> bs;
			
			//se define un largo aleatorio entre el minimo y el maximo ingresado
			int aleatorio = informacion[i].min_length+rand()%(informacion[i].max_length-informacion[i].min_length);
			//generacion de rutas iniciales
			for(int j=0;j<aleatorio;j++)
			{
				int stop_random=rand()%bus_stops.size();
				if(j!=0)
				{
					//verifica que la ruta sea conexa
					while(travel_times[bs[j-1].idi][stop_random]==-1)
					{
						stop_random=rand()%bus_stops.size();
					}
				}
				bs.push_back(bus_stops[stop_random]);
			}
			r->set_bus_stops(bs);
			r->define_bus_type(informacion[i].tipo_ruta);
			
			//solo se consideran rutas que no tengan ciclos ni backtracks
			if(!r->check_cycles_and_backtracks())
			{
				rts.push_back(r[0]);
				cantidad++;
			}
			delete r;
			
		}
		s->set_routes(rts);
	}
	return s;
}	

void Inmune::generar_poblacion(SolutionSet &poblacion,int pop_size,vector<BusStop> &bus_stops,int** &travel_times)
{
	for(int j=0;j<pop_size;j++)
	{
		Solution *s = generar_anticuerpo(bus_stops,travel_times,j*time(NULL));
		
		cout << "antes " << poblacion.solutions.size() << endl;
		
		poblacion.solutions.push_back(*s);
		
		cout << "despues " << poblacion.solutions.size() << endl;
		
		delete s;
	}
	
}

void Inmune::evaluar_fo(SolutionSet* &poblacion,int** &travel_times, int** &demand,ShortestRoute* &sr,int size)
{
	for(int i=0;i<poblacion->solutions.size();i++)
	{
		float fo1 = poblacion->solutions[i].setFO1(sr,demand);
		float fo2 = poblacion->solutions[i].setF02(size,travel_times);
	}
}


bool Inmune::es_dominado_de_Pareto(Solution s, SolutionSet ss){
	bool dominado=false;
	for(int i=0;i<ss.solutions.size();i++)
	{
		if(s.fo1>ss.solutions[i].fo1 && s.fo2>ss.solutions[i].fo2)
		{
			dominado =  true;
		}
	}
	return dominado;
}

void Inmune::afinidad(SolutionSet* &poblacion, float alpha, float beta){
	for(int i=0;i<poblacion->solutions.size();i++)
	{
		poblacion->solutions[i].setQuality(alpha,beta);
	}
}

void Inmune::eliminar_dominados(SolutionSet* &poblacion)
{
	int size=poblacion->solutions.size();
	vector<bool> marcar;
	for(int i=0;i<size;i++)
	{
		if(es_dominado_de_Pareto(poblacion->solutions[i],*poblacion))
		{
			marcar.push_back(true);
		}
		else
		{
			marcar.push_back(false);
		}
	}
	
	for(int i=size-1;i>=0;i--)
	{
		if(marcar[i])
		{
			poblacion->solutions.erase(poblacion->solutions.begin()+i);
		}
	}
}

vector<float> Inmune::ordenar_afinidad(SolutionSet* &poblacion)
{
	vector<float> afinidades;
	for(int i=0;i<poblacion->solutions.size();i++)
	{
		float afinidad = poblacion->solutions[i].quality;
		afinidades.push_back(afinidad);
	}
	sort(afinidades.begin(),afinidades.end());
	
	return afinidades;
}

int Inmune::get_index(float &valor, SolutionSet ss)
{
	for(int i=0;i<ss.solutions.size();i++)
	{
		if(ss.solutions[i].quality==valor)
		{
			return i;
		}
	}
	return -1;
}

vector<Solution> Inmune::seleccionar_mejores_anticuerpos(SolutionSet* &poblacion,float porcentaje)
{
	vector<Solution> mejores;
	int seleccion = poblacion->solutions.size()*porcentaje;
	vector<float> afinidades = ordenar_afinidad(poblacion);
	
	for(int i=0;i<seleccion;i++)
	{
		int index = get_index(afinidades[i],*poblacion);
		mejores.push_back(poblacion->solutions[index]);
	}
	return mejores;
}

void Inmune::clonar_anticuerpos(vector<Solution> &clones,int n_clones)
{
	int random;
	int size = clones.size();
	srand(time(NULL));
	for(int i=0;i<n_clones-size;i++)
	{
		random = rand()%size;
		clones.push_back(clones[random]);
	}
}


void Inmune::mutacion(vector<Solution> &clones,int seed, float probabilidad,vector<BusStop> &bus_stops,int** &travel_times)
{
	int posicion,clon_size,size,tipo;
	srand(seed);
	clon_size=clones.size();
	for(int i=0;i<clon_size;i++)
	{
		//si el numero aleatorio es mayor que la probabilidad entonces se muta al clon
		if(rand()%101>probabilidad*100)
		{
			size = clones[i].routes.size();
			posicion =  rand()%size;
			tipo= clones[i].routes[posicion].tipo_ruta;
			
			std::vector<BusStop> bs;
			
			bs.clear();
			//se define un largo aleatorio entre el minimo y el maximo ingresado
			int aleatorio = informacion[tipo].min_length+rand()%(informacion[tipo].max_length-informacion[tipo].min_length);
			//generacion de rutas iniciales
			for(int j=0;j<aleatorio;j++)
			{
				int stop_random=rand()%bus_stops.size();
				if(j!=0)
				{
					//verifica que la ruta sea conexa
					while(travel_times[bs[j-1].idi][stop_random]==-1)
					{
						stop_random=rand()%bus_stops.size();
					}
				}
				bs.push_back(bus_stops[stop_random]);
			}
			clones[i].routes[posicion].set_bus_stops(bs);
		}
	}
}

bool Inmune::valor_in_vector(float valor,vector<float> v)
{
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==valor)
		{
			return true;
		}
	}
	return false;
}

void Inmune::eliminar_exceso(vector<Solution> &clones,int pop_size,float porcentaje,int** &travel_times, int** &demand,ShortestRoute* &sr,int size, float alpha, float beta)
{
	vector<float> afinidades;
	int clones_size = clones.size();
	//se actualizan las funciones objetivo y la calidad de los clones
	for(int i=0;i<clones_size;i++)
	{
		float fo1 = clones[i].setFO1(sr,demand);
		float fo2 = clones[i].setF02(size,travel_times);
		clones[i].setQuality(alpha,beta);
		afinidades.push_back(clones[i].quality);
	}
	
	sort(afinidades.begin(),afinidades.end());
	
	//se escoge un porcentaje de los mejores clones
	int n_clones = pop_size*porcentaje;
	afinidades.erase(afinidades.begin()+n_clones,afinidades.end());
	
	
	for(int i=clones_size-1;i>=0;i--)
	{
		if(!valor_in_vector(clones[i].quality,afinidades))
		{
			clones.erase(clones.begin()+i);
		}
	}
}
		
void Inmune::nueva_generacion(SolutionSet* &poblacion,vector<Solution> &clones,int pop_size, int reemplazo,vector<BusStop> &bus_stops,int** &travel_times,int** &demand,ShortestRoute* &sr, int size,float alpha, float beta)
{	
	for(int i=0;i<clones.size();i++)
	{
		//se agregan los clones sobrevivientes
		poblacion->solutions.push_back(clones[i]);
	}
	
	vector<float> afinidades = ordenar_afinidad(poblacion);
	
	if(afinidades.size()>pop_size)
	{
		afinidades.erase(afinidades.begin()+pop_size,afinidades.end());
		
		for(int i=0;i<poblacion->solutions.size();i++)
		{
			if(!valor_in_vector(poblacion->solutions[i].quality,afinidades))
			{
				poblacion->solutions.erase(poblacion->solutions.begin()+i);
			}
		}
	}
	else
	{
		for(int i=0;i<pop_size-afinidades.size();i++)
		{
			Solution *s = generar_anticuerpo(bus_stops,travel_times,i*time(NULL));
			float fo1 = s->setFO1(sr,demand);
			float fo2 = s->setF02(size,travel_times);
			s->setQuality(alpha,beta);
			poblacion->solutions.push_back(*s);
			delete s;
		}
	}
	
	//se reemplzan los peores anticuerpos por anticuerpos generados aleatoriamente
	int n_reemplazo = reemplazo*pop_size;
	
	poblacion->solutions.erase(poblacion->solutions.end()-n_reemplazo,poblacion->solutions.end());
	
	for(int i=0;i<n_reemplazo;i++)
	{
		Solution *s = generar_anticuerpo(bus_stops,travel_times,i*time(NULL));
		float fo1 = s->setFO1(sr,demand);
		float fo2 = s->setF02(size,travel_times);
		s->setQuality(alpha,beta);
		poblacion->solutions.push_back(*s);
		delete s;
	}
}
			
