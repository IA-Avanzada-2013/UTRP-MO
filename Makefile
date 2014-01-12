OPTS := -DSOMEOPTION

all: utrpmo

utrpmo: main.o
	g++ $(OPTS) -o utrpmo main.o common.o DataHandler.o Problem.o Solution.o BusStop.o Route.o RouteInfo.o ShortestRoute.o SolutionSet.o Utils.o hv.o avl.o inmune.o opciones.o

main.o: main.cpp common.o DataHandler.o Problem.o Solution.o BusStop.o Route.o RouteInfo.o ShortestRoute.o SolutionSet.o Utils.o hv.o avl.o inmune.o opciones.o
	g++ $(OPTS) -c main.cpp

common.o: common.cpp common.h
	g++ $(OPTS) -c common.cpp

DataHandler.o: DataHandler.cpp DataHandler.h common.h
	g++ $(OPTS) -c DataHandler.cpp

Problem.o: Problem.cpp Problem.h common.h
	g++ $(OPTS) -c Problem.cpp

Solution.o: Solution.cpp Solution.h common.h
	g++ $(OPTS) -c Solution.cpp

BusStop.o: BusStop.cpp BusStop.h common.h
	g++ $(OPTS) -c BusStop.cpp

Route.o: Route.cpp Route.h common.h
	g++ $(OPTS) -c Route.cpp

RouteInfo.o: RouteInfo.cpp RouteInfo.h common.h
	g++ $(OPTS) -c RouteInfo.cpp

ShortestRoute.o: ShortestRoute.cpp ShortestRoute.h common.h
	g++ $(OPTS) -c ShortestRoute.cpp

SolutionSet.o: SolutionSet.cpp SolutionSet.h common.h
	g++ $(OPTS) -c SolutionSet.cpp

Utils.o: Utils.cpp Utils.h SolutionSet.h Problem.h common.h
	g++ $(OPTS) -c Utils.cpp
	
hv.o: hv/hv.c hv/hv.h hv/avl.h
	gcc $(OPTS) -c hv/hv.c
	
avl.o: hv/avl.c hv/avl.h 
	gcc $(OPTS) -c hv/avl.c
	
inmune.o: inmune.cpp inmune.h 
	gcc $(OPTS) -c inmune.cpp

opciones.o: opciones.cpp opciones.h
	gcc $(OPTS) -c opciones.cpp


clean:
	rm -rf *.o utrpmo
