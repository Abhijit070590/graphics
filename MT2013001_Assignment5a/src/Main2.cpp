
#include <iostream>
#include <fstream>
#include "../Header/Mesh.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
using namespace std;

Mesh *mesh;
//int numVert;
int level = 0;  //subdivision level
//vector<int> vertInd1, vertInd2, vertNewIndx;
float lookAngle_x = 45.0, lookAngle_y = -10.0;

//-- A temperorary structure ---
// struct faceNew 
// {
//   int vi[3];
// };

//---- Function to determine if a vertex has already been inserted on an edge ----
// int search(int i1, int i2)
// {
//     int size = vertInd1.size();
//     if(size == 0) return -1;
//     for(int i = 0;  i < size; i++)
//     {
//         if(vertInd1[i] == i2  && vertInd2[i] == i1) return vertNewIndx[i];
//     }
//     return -1;
// }

// //---- Function to subdivide the mesh ---------------------------------------------
// //---- Call this function for each iteration --------------------------------------
// void subdivide()
// {
//     int nsize;
//     vector<Vertex*> vertNewArray;  //Temporary array holding transformed and new vertices
//     vector<faceNew*> faceNewArray;
//     vector<faceNew*> faceOldArray;

// 	Vertex *vert1, *vert2, *vert3, *vert4, *vertNew;
// 	Edge *edge;
// 	vector<int> neighbs;
//     numVert = mesh->_vertArray.size();
//     float wt1 = 0.625, wt2;
// 	float sumx, sumy, sumz;
// 	//Update existing vertices  --- Eq. (8.20) ----
//     for(int i = 0;  i < numVert; i++)
//     {
//         neighbs = mesh->oneRingNbs(i);
//         nsize = neighbs.size();
//         if(nsize == 3)       wt2 = 0.125;  //*3 = 0.375
//         else if(nsize == 6)  wt2 = 0.0625; //*6 = 0.375
// 		vert1 = mesh->_vertArray[i];
// 		sumx = 0.625*(vert1->x);
// 		sumy = 0.625*(vert1->y);
// 		sumz = 0.625*(vert1->z);
// 		for(int k = 0;  k < nsize; k++)
// 		{
// 			vert2 = mesh->_vertArray[neighbs[k]];
// 			sumx += wt2*(vert2->x);
// 			sumy += wt2*(vert2->y);
// 			sumz += wt2*(vert2->z);
// 		}
// 		vertNew = new Vertex;
// 		vertNew->x = sumx;  vertNew->y = sumy;  vertNew->z = sumz;
// 		vertNew->index = i;  vertNew->edge = vert1->edge;
// 		vertNewArray.push_back(vertNew);
//         neighbs.clear();
//     }
	
// 	int vsize = numVert;
// 	vertInd1.clear();
// 	vertInd2.clear();
// 	vertNewIndx.clear();
// 	Face *face;
//     faceNew *fnew, *fold;
//     list<Face*>::iterator f_iter;
// 	int vindx;  //edge vertex indices
//     //Create new vertices
// 	for(f_iter = mesh->_faceList.begin(); f_iter != mesh->_faceList.end(); f_iter++)
// 	{
// 		face = *f_iter;
// 		edge = face->edge;
//         fnew = new faceNew;
// 		fold = new faceNew;
// 		for(int k = 0;  k < 3;  k++)   //Assumption: Triangular mesh
// 		{
// 			vert1 = edge->vert;
// 			fold->vi[k] = vert1->index;
// 			vert2 = edge->pair->vert;
//             vindx = search(vert1->index,  vert2->index);
// 			if( vindx < 0)  //create a new vertex
// 			{
// 				vertInd1.push_back(vert1->index);
// 				vertInd2.push_back(vert2->index);
//                 vertNewIndx.push_back(vsize);
// 				vertNew = new Vertex;
// 				vert3 = edge->next->vert;
// 				vert4 = edge->pair->next->vert;
//                 //--Eq. (8.19)--
// 				vertNew->x = 0.375*((vert1->x)+(vert2->x)) + 0.125*((vert3->x)+(vert4->x));
// 				vertNew->y = 0.375*((vert1->y)+(vert2->y)) + 0.125*((vert3->y)+(vert4->y));
// 				vertNew->z = 0.375*((vert1->z)+(vert2->z)) + 0.125*((vert3->z)+(vert4->z));
// 				vertNew->index = vsize;
//                 fnew->vi[k] = vsize;
// 				vertNew->edge = edge;
// 				vertNewArray.push_back(vertNew);
// 				vsize++;
// 			}
//             else   fnew->vi[k] = vindx;   //new vertex already created
// 			edge = edge->next;
// 		}
//         faceNewArray.push_back(fnew);
// 		faceOldArray.push_back(fold);
// 	}

//     //Restructure and retriangulate each face
// 	Edge *e[3], *enew1, *enew2, *enew3;
// 	Face *f[3];
// 	Vertex *vn[3], *vo[3];
// 	int k1, k2;

// 	int fsize = mesh->_faceList.size();
// 	f_iter = mesh->_faceList.begin();
// 	for(int findx = 0;  findx < fsize; findx++)
// 	{
// 		f[0] = *f_iter;    //current face
// 		f[1] = new Face; //Create 2 more sub-faces
// 		f[2] = new Face;
// 		e[0] = f[0]->edge;
// 		e[1] = e[0]->next;
// 		e[2] = e[1]->next;
//         fnew = faceNewArray[findx];
// 		fold = faceOldArray[findx];
// 		for(int k = 0;  k < 3;  k++)  
// 		{
//             vindx = fnew->vi[k];
//             vn[k] = vertNewArray[vindx];
// 			vindx = fold->vi[k];
// 			vo[k] = vertNewArray[vindx];
// 		}
// 		for(int k = 0;  k < 3;  k++)  
// 		{
// 			enew1 = new Edge;  //Create 2 new edges for each sub-face
// 			enew2 = new Edge;
//             e[k]->vert = vn[k]; e[k]->face = f[k];
// 			e[k]->next = enew1; e[k]->prev = enew2;
// 			k1 = (k+2)%3;
// 			enew1->vert = vn[k1]; enew1->prev = e[k]; enew1->next = enew2; enew1->face = f[k];
// 			enew2->vert = vo[k1]; enew2->prev = enew1; enew2->next = e[k]; enew2->face = f[k];
// 			f[k]->edge = e[k];
// 			vo[k1]->edge = enew2;
// 			mesh->_edgeList.push_back(enew1);
// 			mesh->_edgeList.push_back(enew2);
// 		}
// 		mesh->_faceList.push_back(f[1]);
// 		mesh->_faceList.push_back(f[2]);
// 		//Create middle face
// 		face = new Face;
// 		enew1 = new Edge;  //Create 3 new edges for each sub-face
// 		enew2 = new Edge;
// 		enew3 = new Edge;
// 		enew1->vert = vn[0]; enew1->prev = enew3; enew1->next = enew2; enew1->face = face;
// 		enew2->vert = vn[1]; enew2->prev = enew1; enew2->next = enew3; enew2->face = face;
// 		enew3->vert = vn[2]; enew3->prev = enew2; enew3->next = enew1; enew3->face = face;
// 		mesh->_edgeList.push_back(enew1);
// 		mesh->_edgeList.push_back(enew2);
// 		mesh->_edgeList.push_back(enew3);
// 		face->edge = enew1;
// 		mesh->_faceList.push_back(face);
// 		f_iter++;
// 	}
// 	mesh->_vertArray.clear();
// 	for(int i = 0;  i < vsize; i++) mesh->_vertArray.push_back(vertNewArray[i]);

// 	list<Edge*>::iterator e_iter;
// 	for(e_iter = mesh->_edgeList.begin(); e_iter != mesh->_edgeList.end(); e_iter++) (*e_iter)->pair = NULL;

//     mesh->_numVert = vsize;
// 	vertNewArray.clear();
// 	faceNewArray.clear();
// 	faceOldArray.clear();
// 	mesh->createEdgePairs();
// }

//---Reads data file and initializes drawing parameters ------------------
void initialise()
{
	mesh = new Mesh();
	mesh->load_mesh("cube.ply");
	//mesh->triangulation();
	mesh->make_edges();
   // mesh->setColorAmbientDiffuse(1.0, 0.5, 0.5);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2., 2., -2., 2., -2., 2.);
}

//---- Display:  The main display module ----------------------------------
void display()  
{
    float lgt_pos[]={5.0f, 10.0f, 10.0f, 1.0f};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  	
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);

	glRotatef(lookAngle_y, 0, 1, 0);
	glRotatef(lookAngle_x, 1, 0, 0);
	glColor3f(1.,0.,0.);
	glScalef(7.0,7.0,7.0);
	mesh->draw();
	glFlush();
}

//-- Callback function for processing keyboard events -------------------- 
void keyboard(unsigned char key, int x, int y)
{
	if (key == ' ' && level < 4)
	{
		mesh->subdivide();
		level++;
	}
	else if(key == 'w' || key == 'W') {glDisable(GL_LIGHTING); glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
	else if(key == 's' || key == 'S') {glEnable(GL_LIGHTING); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
	glutPostRedisplay();
}

//-- Callback function for processing special keyboard events --------------	
void special(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) lookAngle_y -= 5.0;
	else if(key == GLUT_KEY_RIGHT) lookAngle_y += 5.0;
	if(key == GLUT_KEY_UP) lookAngle_x -= 5.0;
	else if(key == GLUT_KEY_DOWN) lookAngle_x += 5.0;
	glutPostRedisplay();
}

//------- Main: Initialize glut window and register call backs -------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Loop Subdivision");
   initialise ();

   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
