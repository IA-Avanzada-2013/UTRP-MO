OPTS := -DSOMEOPTION

all: utrpmo

utrpmo: main.o
	g++ $(OPTS) -o utrpmo main.o DataHandler.o

main.o: main.cpp DataHandler.o
	g++ $(OPTS) -c main.cpp

DataHandler.o: DataHandler.cpp DataHandler.h common.h
	g++ $(OPTS) -c DataHandler.cpp

# Item.o: Item.cpp Item.h Utils.o
# 	g++ $(OPTS) -c Item.cpp

# Strip.o: Strip.cpp Strip.h Utils.o
# 	g++ $(OPTS) -c Strip.cpp

# Utils.o: Utils.cpp Utils.h
# 	g++ $(OPTS) -c Utils.cpp

clean:
	rm -rf *.o utrpmo
