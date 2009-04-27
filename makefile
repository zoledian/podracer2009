
all : 	main

main : 	main.cpp glstates.cpp
	g++ -Wall -o podracer2009 -lglut -lGL main.cpp glstates.cpp input.cpp

clean :	rm podracer2009
