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
    Route * initial = &routes[0];
    int k=-1;
    //Si no hay nodo intermedio retornar cero transferencias.
    if( next[a][b] == -1 ) return 0;
    k = next[a][b];

    //Buscamos una ruta en que se encuentren ambos nodos.
    for(unsigned int i=0; i<routes.size(); i++){
        //Si estan nodos a,b,k.
          if( routes[i].find_nodes(a,b) && routes[i].find_node(k) ){ initial = &routes[i]; break; }
        //Si no, si estan nodos a,k o b,k
          if( routes[i].find_nodes(a,k) ||  routes[i].find_nodes(b,k) ){ initial = &routes[i] ; break; }
    }

    return transfer(a,b, routes,  initial );
}

/*ShortestRoute::transfers
Función auxiliar. Retorna el número de transferencias entre dos nodos usando recursividad.
Ve si es que es necesaria una transferencia o no.
@args:
    int a,b: los nodos a los que se calculara las transferencias.
    std::vector<Route> routes: el set de rutas respecto del cual se veran las transferencias.
    Route *actual: la ruta por la que se va moviendo.
*/
int ShortestRoute::transfer(int a, int b, std::vector<Route> routes, Route *actual){
    Route *ruta_a = actual, *ruta_b = actual;
    int t=0, k=-1;

    //Caso base. Si no hay nodo intermedio retornar cero transferencias.
    if(next[a][b] == -1) return 0;
    //Si no, significa que se debe pasar por el nodo k antes.
    k = next[a][b];

    //Vemos si en la ruta estan ambos nodos k y a.
    if( !(*actual).find_nodes(a,k) ){
        //De no estar debemos buscar otra ruta en que esten
        for(unsigned int i=0; i<routes.size(); i++){
            if( routes[i].find_nodes(a,k) ){ ruta_a = &routes[i]; t++ ; break; }
        }
    }

    //Vemos si en la ruta estan ambos nodos k y b.
    if( !(*actual).find_nodes(b,k) ){
        //De no estar debemos buscar otra ruta en que esten
        for(unsigned int i=0; i<routes.size(); i++){
            if( routes[i].find_nodes(b,k) ){ ruta_b = &routes[i]; t++ ; break; }
        }
    }

    return transfer(a,k,routes,ruta_a) + t + transfer(k,a,routes,ruta_b) ;
}
