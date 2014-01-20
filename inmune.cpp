#include "inmune.h"

using namespace std;

Inmune::Inmune(vector<RouteInfo> info,Opciones* &opc,int** &demanda, int** &travel,vector<BusStop> &bstops,int &tam,int &semilla)
{
	this->informacion = info;
	this->opciones = opc;
	this->demand = demanda;
	this->travel_times = travel;
	this->bs = bstops;
	this->size = tam;
	this->seed = semilla;
	ShortestRoute *shrt = new ShortestRoute(this->size);
	shrt->calcDistNoRoutes(this->travel_times);
	this->sr = shrt;
}	

vector<int> Inmune::get_vecinos(int id)
{
	vector<int> v;
	for(int i=0;i<this->opciones.get_size();i++)
	{
		if(id!=i && this->travel_times[id][i]!=-1)
		{
			v.push_back(i);
		}
	}
}



vector<int> Inmune::paradas_disponibles()
{
	vector<int> v;
	for(int i=0;i<this->bs.size();i++)
	{
		v.push_back(this->bs[i].idi);
	}
	
	return v;
}

Route* Inmune::generar_ruta_factible(int tipo)
{
	int cantidad=0;
	Route *r;

	while(cantidad<1)
	{
		r = new Route();
		std::vector<BusStop> bus;
			
		//se define un largo aleatorio entre el minimo y el maximo ingresado
		int aleatorio = informacion[tipo].min_length+rand()%(informacion[tipo].max_length-informacion[tipo].min_length);
		//cout << "aleatorio " << aleatorio << endl;
		//generacion de rutas iniciales
		for(int j=0;j<aleatorio;j++)
		{
			int stop_random=rand()%this->bs.size();
			
			if(j!=0)
			{
				//verifica que la ruta sea conexa
				while(this->travel_times[bus[j-1].idi][stop_random]==-1 || this->travel_times[bus[j-1].idi][stop_random]==0)
				{
					stop_random=rand()%this->bs.size();
				}
			}
			bus.push_back(this->bs[stop_random]);
		}
		r->set_bus_stops(bus);
		r->define_bus_type(tipo);
		
		//solo se consideran rutas que no tengan ciclos ni backtracks
		if(!r->check_cycles_and_backtracks())
		{
			//cout << "agregada" << endl;
			cantidad++;
		}
		else
		{
			delete r;
		}
	}
	return r;
}

vector<BusStop> Inmune::invertir(vector<BusStop> stops)
{
	vector<BusStop> nuevo;
	
	for(int i=stops.size()-1;i>0;i--)
	{
		nuevo.push_back(stops[i]);
	}
	return nuevo;
}

void Inmune::movimiento_factible(Route &r)
{
	if(r.bus_stops.size()<this->informacion[r.tipo_ruta].max_length)
	{
		//agregar una nueva parada
		for(int i=0;i<this->bs.size();i++)
		{
			int ultimo = r.bus_stops.size()-1;
			if(!r.find_node(this->bs[i].idi) && this->travel_times[ultimo][this->bs[i].idi] != 1)
			{
				r.bus_stops.push_back(this->bs[i]);
				break;
			}
		}
	}
	else
	{
		//invertir
		//vector<BusStop> inverso = invertir(r.bus_stops);
		//r.set_bus_stops(inverso);
		if(r.bus_stops.size()>this->informacion[r.tipo_ruta].min_length)
		{
			//r.bus_stops.pop_back();
		}
	}
}

Solution* Inmune::generar_anticuerpo()
{
	//inicializacion de la solucion
	Solution *s = new Solution();
	std::vector<Route> rts;
	
	for(unsigned int i=0;i<this->informacion.size();i++)
	{
		int cantidad=0;
		while(cantidad<this->informacion[i].quantity)
		{
			Route *r = generar_ruta_factible(this->informacion[i].tipo_ruta);
			cantidad++;
			rts.push_back(*r);
			delete r;
			
		}
		s->set_routes(rts);
	}
	return s;
}	

void Inmune::generar_poblacion(SolutionSet &poblacion)
{
	int cantidad,intentos;
	cantidad = 0;
	intentos = 10*this->opciones.get_popsize();
	while(cantidad<this->opciones.get_popsize())
	{
		Solution *s = generar_anticuerpo();
		if(s->check_connectivity(this->opciones.get_size()))
		{
			poblacion.solutions.push_back(*s);
			cantidad++;
		}	
		//cout << cantidad << endl;
		delete s;
		intentos++;
		//cout << intentos << endl;
		if(cantidad>intentos*1000)
		{
			break;
		}
	}
	
}

void Inmune::evaluar_fo(SolutionSet* &poblacion)
{
	for(int i=0;i<poblacion->solutions.size();i++)
	{
		float fo1 = poblacion->solutions[i].setFO1(this->sr,this->demand);
		float fo2 = poblacion->solutions[i].setF02(this->size,this->travel_times);
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

void Inmune::afinidad(SolutionSet* &poblacion){
	for(int i=0;i<poblacion->solutions.size();i++)
	{
		poblacion->solutions[i].setQuality(this->opciones.get_alpha(),this->opciones.get_beta());
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

int Inmune::get_index(float &valor, SolutionSet* ss)
{
	for(int i=0;i<ss->solutions.size();i++)
	{
		if(ss->solutions[i].quality==valor)
		{
			return i;
		}
	}
	return -1;
}

vector<Solution> Inmune::seleccionar_mejores_anticuerpos(SolutionSet* &poblacion)
{	
	vector<Solution> mejores;
	//se elige un porcentaje de los mejores individuos
	int seleccion = poblacion->solutions.size()*this->opciones.get_afinidad();
	vector<float> afinidades = ordenar_afinidad(poblacion);
	
	for(int i=0;i<seleccion;i++)
	{
		int index = get_index_solucion(afinidades[i],poblacion->solutions);
		mejores.push_back(poblacion->solutions[index]);
	}
	return mejores;
}

void Inmune::clonar_anticuerpos(vector<Solution> &clones)
{
	int random;
	int size = clones.size();
	srand(time(NULL));
	for(int i=0;i<this->opciones.get_clonsize()-size;i++)
	{
		random = rand()%size;
		clones.push_back(clones[random]);
	}
}

void Inmune::mutar(Solution &clon)
{
	int posicion = rand()%clon.routes.size();
	int tipo = clon.routes[posicion].tipo_ruta;
	bool flag = false;
	vector<int> paradas;
	for(int j=0;j<size;j++)
	{
		flag = false;
		for(int i=0;i<clon.routes.size();i++)
		{
			if(i!=posicion)
			{
				if(clon.routes[i].find_node(j))
				{
					flag = true;
				}
			}
		}
		if(!flag)
		{		
			paradas.push_back(j);
		}
	}
}


void Inmune::mutacion(vector<Solution> &clones)
{
	int posicion,clon_size,size,tipo;
	clon_size=clones.size();
	bool flag = true;
	for(int i=0;i<clon_size;i++)
	{
		posicion = rand()%clones[i].routes.size();
		
		//movimiento_factible(clones[i].routes[posicion]);		
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

void Inmune::borrar_parada(int stop,vector<int> &v)
{
	int borrar=-1;
	for(int i=0;i<v.size();i++)
	{
		if(stop==v[i])
		{
			borrar = i;
		}
	}
	
	if(borrar!=-1)
	{
		v.erase(v.begin()+borrar);
	}
}

int Inmune::get_index_solucion(float valor,vector<Solution> clones)
{
	int index = 0;
	for(int i=0;i<clones.size();i++)
	{
		if(clones[i].quality==valor)
		{
			index = i;
		}
	}
	return index;
}

void Inmune::eliminar_exceso(vector<Solution> &clones)
{
	vector<float> afinidades;
	int clones_size = clones.size();
	vector<Solution> copia = clones;
	//se actualizan las funciones objetivo y la calidad de los clones
	for(int i=0;i<clones_size;i++)
	{
		float fo1 = clones[i].setFO1(this->sr,this->demand);
		float fo2 = clones[i].setF02(this->size,this->travel_times);
		clones[i].setQuality(this->opciones.get_alpha(),this->opciones.get_beta());
		if(!valor_in_vector(clones[i].quality,afinidades))
		{
			afinidades.push_back(clones[i].quality);
			//cout << clones[i].quality << endl;
		}
		
	}
	
	sort(afinidades.begin(),afinidades.end());
	clones.clear();
	//se escoge un porcentaje de los mejores clones

	int n_clones = this->opciones.get_clonsize()*this->opciones.get_porcentajeclones();
	afinidades.erase(afinidades.begin()+n_clones,afinidades.end());

	int index =0;
	for(int i=0;i<afinidades.size();i++)
	{
		index = get_index_solucion(afinidades[i],copia);
		clones.push_back(copia[index]);
	}
}
		
void Inmune::nueva_generacion(SolutionSet* &poblacion,vector<Solution> &clones)
{	
	vector<Solution> nueva_generacion;
	for(int i=0;i<clones.size();i++)
	{
		//se agregan los clones sobrevivientes
		poblacion->solutions.push_back(clones[i]);
	}
	

	vector<float> afinidades = ordenar_afinidad(poblacion);
	
	afinidades.erase(afinidades.begin()+this->opciones.get_popsize(),afinidades.end());
	 
	for(int i=0;i<afinidades.size();i++)
	{
		int index = get_index_solucion(afinidades[i],poblacion->solutions);
		nueva_generacion.push_back(poblacion->solutions[index]);
	}
	
	poblacion->solutions = nueva_generacion;
	
	//se reemplzan los peores anticuerpos por anticuerpos generados aleatoriamente
	int n_reemplazo = this->opciones.get_porcentajereemplazo()*this->opciones.get_popsize();
	
	poblacion->solutions.erase(poblacion->solutions.end()-n_reemplazo,poblacion->solutions.end());
	int cantidad = 0;
	while(cantidad<n_reemplazo)
	{
		Solution *s = generar_anticuerpo();
		if(s->check_connectivity(this->opciones.get_size()))
		{
			poblacion->solutions.push_back(*s);
			float fo1 = s->setFO1(this->sr,this->demand);
			float fo2 = s->setF02(this->size,this->travel_times);
			s->setQuality(this->opciones.get_alpha(),this->opciones.get_beta());	
			cantidad++;
		}	
		
		delete s;
	}
}
			
