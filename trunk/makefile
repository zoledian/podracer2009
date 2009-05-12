OBJECTS :=  main.o glstates.o input.o camera.o ship.o block.o loadlevel.o skybox.o
CC = g++ -g -Wall 
name = podracer2009
version = 0.1

podracer2009 : ${OBJECTS}
	$(CC) -lglut -lGL -o $(name)-$(version) $(OBJECTS)

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

loadlevel.o : loadlevel.cpp loadlevel.h
	$(CC)  -c loadlevel.cpp

skybox.o : skybox.cpp skybox.h
	$(CC)  -c skybox.cpp

clean :
	rm -Rf $(name)-$(version) $(OBJECTS)