exec: bunny
	./bunny

bunny: load1.o quaternion.o Vector.o 
	g++ -o bunny load1.o quaternion.o Vector.o -lGL -lGLU -lglut -lglui -lGLEW -lSOIL -lm

quaternion.o: Header/quaternion.h src/quaternion.cpp
	g++ -c -o quaternion.o src/quaternion.cpp

Vector.o: Header/Vector.h src/Vector.cpp
	g++ -c -o Vector.o src/Vector.cpp

load1.o: Header/Vector.h Header/quaternion.h src/load1.cpp
	g++ -c -o load1.o src/load1.cpp

clean : 
	rm *.o bunny
