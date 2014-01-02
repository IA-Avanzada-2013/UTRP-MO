#include "ShortestRoute.h"
#include <iostream>
#include <vector>

#define INFINITY 99999
//Constructor
ShortestRoute::ShortestRoute(int _size){
    size = _size;

    dist = new int*[size];
    next = new int*[size];

    for(int i=0;i<size;i++){
        dist[i] = new int [size];
        next[i] = new int [size];
    }

}

//Destructor
ShortestRoute::~ShortestRoute(){
    for(int i=0;i<size;i++){
        delete [] dist[i];
        delete [] next[i];
    }
    delete [] dist;
    delete [] next;

}

/* ShortestRoute::calcDist
Aplica el algoritmo de Floyd para encontrar las distancias más cortas respecto del set de rutas.
@args:
    int **&travel_times: matriz de costos
    std::vector<Route>: set de rutas

*/
void ShortestRoute::calcDist(int **&travel_time, std::vector<Route> routes ){
    bool in_routes = false;

    //Distancia de un nodo asi mismo es cero
    for(int i=0;i<size;i++){
        dist[i][i] = 0;
    }

    //Inicializamos la matriz con las distancias entre los nodos.
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            in_routes = false;

            //Se verifica si es que cada arco esta en el set de rutas.
            for(unsigned int k=0; k<routes.size();k++ ){
                if( routes[k].check_edge(i,j) ){in_routes = true; break;}
            }
            if(i == j) in_routes = true;

            //Si el arco esta en el set de rutas y hay un arco entonces
            if(travel_time[i][j] >= 0 && in_routes ) dist[i][j] = travel_time[i][j];
            //Si no la distancia es infinita
            else dist[i][j] = INFINITY;

            //Se asume que para llegar de un nodo a otro no se debe pasar por ninguno intermedio
            next[i][j] = -1;

        }
    }

    //Algoritmo de Floyd.
    for(int k=0;k<size;k++){
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(dist[i][j] > dist[i][k] + dist[k][j] ){
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = k;
                }
            }
        }
    }
}

/* ShortestRoute::calcDist
Aplica el algoritmo de Floyd para encontrar las distancias más cortas respecto del problema
@args:
    int **&travel_times: matriz de costos
*/
void ShortestRoute::calcDistNoRoutes(int **&travel_time){

    //Distancia de un nodo asi mismo es cero
    for(int i=0;i<size;i++){
        dist[i][i] = 0;
    }

    //Inicializamos la matriz con las distancias entre los nodos.
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){

            //Si los nodos son iguales no se hace nada
            if(i == j) continue;

            //Si hay un arco entonces se agrega
            if(travel_time[i][j] >= 0) dist[i][j] = travel_time[i][j];
            //Si no la distancia es infinita
            else dist[i][j] = INFINITY;

            //Se asume que para llegar de un nodo a otro no se debe pasar por ninguno intermedio
            next[i][j] = -1;

        }
    }

    //Algoritmo de Floyd.
    for(int k=0;k<size;k++){
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(dist[i][j] > dist[i][k] + dist[k][j] ){
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = k;
                }
            }
        }
    }
}

void ShortestRoute::showDist(){
    std::cout << "\nShortest Distances" << "\n";

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            std::cout << dist[i][j] << "\t";
        }
        std::cout << "\n";
    }

    std::cout << "\nNexts\n" << "\n";
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            //std::cout << next[i][j] << "\t";
        }
        //std::cout << "\n";
    }
}

int ShortestRoute::getSize(){
    return size;
}

/* ShortestRoute::distance
Retorna la distancia entre a y b.
Llamar a calcDist() antes de usar con un grafo o set de rutas distinto.
@args:
    int a,b: los nodos/paradas a las que se calculara la distancia
@return: distancia entre a y b
*/
int ShortestRoute::distance(int a, int b){
    return dist[a][b];
}

/*ShortestRoute::getTransfers
Retorna el número de transferencias entre dos nodos usando la funcion recursiva transfer.
Llamar a calcDist() antes de usar con un grafo o set de rutas distinto.
@args:
    int a,b: los nodos a los que se calculara las transferencias.
    std::vector<Route> routes: el set de rutas respecto del cual se veran las transferencias.
*/

int ShortestRoute::getTransfers(int a, int b, std::vector<Route> routes){
    int transfers = 0;
    std::list<Route*> rutas_del_camino;
    std::list<int> recorrido;

    if( next[a][b] == -1 ){
            return transfers;
    }

    for(unsigned int i=0;i<routes.size();i++){
        rutas_del_camino.push_back(&routes[i] );
    }

    recorrido.push_back(a);
    recorrido.push_back(b);
    construct_road(a,b, &recorrido);

    std::list<int>::iterator next = recorrido.begin();
    next++;

    for(std::list<int>::iterator it = recorrido.begin(); it != recorrido.end(); it++){

        for(std::list<Route*>::iterator it2 = rutas_del_camino.begin(); it2 != rutas_del_camino.end(); ){
            if( !(**it2).check_edge(*it,*next)  ){
                it2 = rutas_del_camino.erase(it2);
            }else{
                it2++;
            }
        }
        if(rutas_del_camino.empty() ){
            transfers++;
            for(unsigned int i=0;i<routes.size();i++){
                rutas_del_camino.push_back(&routes[i]);
            }
        }

        next++;
        if( next == recorrido.end() ) break;
    }

    return transfers;
}

/*ShortestRoute::construct_road
Retorna el camino m�s corto entre dos nodos. Lo construye en la variable road.
@args:
    int a, int b: los nodos
    list<int> *road: el camino que va construye recursivamente
*/
void ShortestRoute::construct_road(int a, int b, std::list<int> *road){
    int k = next[a][b];
    if( k == -1) return;

    for( std::list<int>::iterator it = (*road).begin(); it != (*road).end() ; it++ ){
        if( *it == b ){
            it = (*road).insert ( it , k);
            break;
        }
    }

    construct_road(a,k, road);
    construct_road(k,b, road);
}

/*ShortestRoute::construct_route
Retorna el camino mas corto entre dos nodos, construyendo en *route las rutas con las
paradas del problema pasadas en el vector bss
@args:
	int a, int b: los nodos
	Route *route: la ruta más corta entre los nodos
	std::vector<BusStop> bss: el vector con las paradas de buses del problema
*/
void ShortestRoute::construct_route(int a, int b, Route *route, std::vector<BusStop> bss ){
	std::list<int> road;
	road.push_back(a);
	road.push_back(b);

	construct_road(a,b,&road);

	(*route).bus_stops.clear();
	for(std::list<int>::iterator it = road.begin(); it != road.end(); it++){
		(*route).bus_stops.push_back( bss[*it] );
	}
}

