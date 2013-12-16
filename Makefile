OPTS := -DSOMEOPTION

all: utrpmo

utrpmo: main.o
	g++ $(OPTS) -o utrpmo main.o common.o DataHandler.o Problem.o Solution.o BusStop.o Route.o RouteInfo.o

main.o: main.cpp common.o DataHandler.o Problem.o Solution.o BusStop.o Route.o RouteInfo.o
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

# Item.o: Item.cpp Item.h Utils.o
# 	g++ $(OPTS) -c Item.cpp

# Strip.o: Strip.cpp Strip.h Utils.o
# 	g++ $(OPTS) -c Strip.cpp

# Utils.o: Utils.cpp Utils.h
# 	g++ $(OPTS) -c Utils.cpp

clean:
	rm -rf *.o utrpmo
