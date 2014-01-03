#ifndef SHORTEST_ROUTE_H
#define SHORTEST_ROUTE_H
#include "common.h"
#include "Route.h"
#include <iostream>
#include <vector>
#include <list>

class ShortestRoute{
    //Distancias más cortas entre los nodos. dist[a][b] indica la distancia más corta.
    int **dist;
    //Nodo intermedio. next[a][b] guarda el nodo de mayor indice por el que se debe pasar para llegar desde 'a' a 'b'
    int **next;
    //Tamaño del grafo.
    int size;

    public:

    ShortestRoute(int _size);
    ~ShortestRoute();
    
    int getSize();
    //Calcula la distancias más cortas entre los nodos
    void calcDist(int **&travel_time, std::vector<Route> routes);
    //Calcula la distancia más corta entre los nodos del problema
    void calcDistNoRoutes(int **&travel_time);
    //Muestra las matrices
    void showDist();

    //Distancia entre nodos (a,b)
    int distance(int a, int b);
    //Numero de transferencias entre los nodos.
    int getTransfers(int a, int b, std::vector<Route> routes);
    //Funcion auxiliar, usada en la recursión al calcular las transferencias.
    //void transfer(int a, int b, std::vector<Route> *routes, Route *actual, std::vector<Route*> *recorridos);
     void construct_road(int a, int b, std::list<int> *road);
    //Funcion auxiliar que permite pasar un camino desde list a vector Route usando construct_road
     void construct_route(int a, int b, Route *route, std::vector<BusStop> bss  );
    
};
#endif
