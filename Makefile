OPTS := -DSOMEOPTION

all: utrpmo

utrpmo: main.o Utils.o Item.o Strip.o
	g++ $(OPTS) -o utrpmo main.o Utils.o Item.o Strip.o

main.o: main.cpp Utils.o Item.o Strip.o
	g++ $(OPTS) -c main.cpp

# Item.o: Item.cpp Item.h Utils.o
# 	g++ $(OPTS) -c Item.cpp

# Strip.o: Strip.cpp Strip.h Utils.o
# 	g++ $(OPTS) -c Strip.cpp

# Utils.o: Utils.cpp Utils.h
# 	g++ $(OPTS) -c Utils.cpp

clean:
	rm -rf *.o utrpmo
