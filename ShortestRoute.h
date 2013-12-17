#ifndef SHORTEST_ROUTE_H
#define SHORTEST_ROUTE_H
#include "common.h"
#include "Route.h"
#include <iostream>
#include <vector>

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
    //Muestra las matrices
    void showDist();

    //Distancia entre nodos (a,b)
    int distance(int a, int b);
    //Numero de transferencias entre los nodos.
    int getTransfers(int a, int b, std::vector<Route> routes);
    private:
    //Funcion auxiliar, usada en la recursión al calcular las transferencias.
    int transfer(int a, int b, std::vector<Route> routes, Route *route);


};
#endif
