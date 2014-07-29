/*
 * test.cpp
 *
 *  Created on: Apr 13, 2014
 *      Author: abhijit
 */




#include "../Header/Mesh.h"
#include <iostream>

int main(int argc, char** argv)
{
	Mesh *mesh = new Mesh();
	mesh = new Mesh();
		mesh->load_mesh("t.ply");
		//mesh->triangulation();
		mesh->make_edges();
		
		mesh->subdivide();
	return 0;
}
