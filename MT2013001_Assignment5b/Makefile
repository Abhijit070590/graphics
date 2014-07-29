subdivide: Mesh.o Main.o 
	g++ -o subdivide Main.o Mesh.o -lGL -lGLU -lglut

Mesh.o: Header/Mesh.h src/Mesh.cpp
	g++ -c -o Mesh.o src/Mesh.cpp

Main.o: Header/Mesh.h src/Main.cpp
	g++ -c -o Main.o src/Main.cpp

clean : 
	rm *.o subdivide
