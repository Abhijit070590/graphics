exec: bunny
	./bunny

bunny: load.o quaternion.o Vector.o
	g++ -o bunny load.o quaternion.o Vector.o -lGL -lGLU -lglut -lGLEW

quaternion.o: Header/quaternion.h src/quaternion.cpp
	g++ -c -o quaternion.o src/quaternion.cpp

Vector.o: Header/Vector.h src/Vector.cpp
	g++ -c -o Vector.o src/Vector.cpp

load.o: Header/Vector.h ./src/load.cpp
	g++ -c -o load.o src/load.cpp

clean : 
	rm *.o bunny
