exec: beiz
	./beiz

beiz: beizeir.o
	g++ -o beiz beizeir.o -lGL -lGLU -lglut
	
beizeir.o: beizeir.h beizeir.cpp
	g++ -c -o beizeir.o beizeir.cpp 

clean : 
	rm *.o beiz 	 
    
