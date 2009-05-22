	OBJECTS :=  main.o camera.o block.o loadlevel.o skybox.o collision.o readjpeg.o ship.o lighting.o shaderutils.o
CC = g++ -g -Wall 
name = podracer2009
version = 0.1

podracer2009 : ${OBJECTS}
	$(CC) -lglut -lGL -ljpeg -o $(name)-$(version) $(OBJECTS)

main.o : main.cpp
	$(CC)  -c main.cpp

block.o : block.cpp block.h
	$(CC)  -c block.cpp

camera.o : camera.cpp camera.h
	$(CC)  -c camera.cpp

ship.o : ship.cpp ship.h
	$(CC)  -c ship.cpp

loadlevel.o : loadlevel.cpp loadlevel.h
	$(CC)  -c loadlevel.cpp

skybox.o : skybox.cpp skybox.h
	$(CC)  -c skybox.cpp

collision.o : collision.cpp collision.h
	$(CC)  -c collision.cpp

readjpeg.o : readjpeg.c readjpeg.h
	gcc -g -Wall -c -shared -fPIC readjpeg.c

shaderutils.o : shaderutils.c shaderutils.h
	gcc -g -Wall -c -shared -fPIC shaderutils.c

lighting.o : lighting.cpp lighting.h
	$(CC)  -c lighting.cpp

clean :
	rm -Rf $(name)-$(version) $(OBJECTS)
