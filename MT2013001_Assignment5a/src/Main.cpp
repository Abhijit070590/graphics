#include <iostream>
#include <fstream>
#include "../Header/Mesh.h"
#include <GL/glut.h>
using namespace std;

Mesh *mesh;
int numVert;
int level = 0;  //subdivision level
vector<int> vertInd1, vertInd2, vertNewIndx;
float lookAngle_x = 45.0, lookAngle_y = -10.0;

//---Reads data file and initializes drawing parameters ------------------
void initialise()
{
	mesh = new Mesh();
	mesh->load_mesh("cube.ply");
	mesh->triangulation();
	mesh->make_edges();
	mesh->make_neighbours();
	mesh->subdivide();
	
    //mesh->setColorAmbientDiffuse(1.0, 0.5, 0.5);
    float col[4] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col);
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
		GLfloat light0_ambient[] =  {0.0f, 0.0f, 0.1f, 0.0f};
		  GLfloat light0_diffuse[] =  {0.0f, 0.0f, 0.0f, 0.0f};
		  GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};

		  glEnable(GL_LIGHTING);
		  glEnable(GL_LIGHT0);
		  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
		  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(lookAngle_y, 0, 1, 0);
		glRotatef(lookAngle_x, 1, 0, 0);
		glColor3f(1.,0.,0.);
		glScalef(0.5,0.5,0.5);
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
