OPTS := -DSOMEOPTION

all: utrpmo

utrpmo: main.o
	g++ $(OPTS) -o utrpmo main.o common.o DataHandler.o Problem.o BusStop.o Route.o

main.o: main.cpp common.o DataHandler.o Problem.o BusStop.o Route.o
	g++ $(OPTS) -c main.cpp

common.o: common.cpp common.h
	g++ $(OPTS) -c common.cpp

DataHandler.o: DataHandler.cpp DataHandler.h common.h
	g++ $(OPTS) -c DataHandler.cpp

Problem.o: Problem.cpp Problem.h common.h
	g++ $(OPTS) -c Problem.cpp

BusStop.o: BusStop.cpp BusStop.h common.h
	g++ $(OPTS) -c BusStop.cpp

Route.o: Route.cpp Route.h common.h
	g++ $(OPTS) -c Route.cpp

# Item.o: Item.cpp Item.h Utils.o
# 	g++ $(OPTS) -c Item.cpp

# Strip.o: Strip.cpp Strip.h Utils.o
# 	g++ $(OPTS) -c Strip.cpp

# Utils.o: Utils.cpp Utils.h
# 	g++ $(OPTS) -c Utils.cpp

clean:
	rm -rf *.o utrpmo
