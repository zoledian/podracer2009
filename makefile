
all : 	main

main : 	main.cpp
	g++ -Wall -o podracer2009 -lglut -lGL main.cpp

clean :	rm podracer2009
