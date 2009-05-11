OBJECTS :=  main.o glstates.o input.o camera.o ship.o block.o
CC = g++ -g -Wall -lglut -lGL 
name = podracer2009
version = 0.1

podracer2009 : ${OBJECTS}
	$(CC) -o $(name)-$(version) $(OBJECTS)

main.o : main.cpp
	$(CC)  -c main.cpp

block.o : block.cpp block.h
	$(CC)  -c block.cpp

camera.o : camera.cpp camera.h
	$(CC)  -c camera.cpp

glstates.o : glstates.cpp glstates.h
	$(CC)  -c glstates.cpp

input.o : input.cpp input.h
	$(CC)  -c input.cpp

ship.o : ship.cpp ship.h
	$(CC)  -c ship.cpp

clean :
	rm -Rf $(name)-$(version) $(OBJECTS)