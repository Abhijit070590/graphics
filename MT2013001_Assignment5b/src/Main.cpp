#include <iostream>
#include <fstream>
#include "../Header/Mesh.h"
#include <GL/glut.h>
using namespace std;

Mesh *smesh,*cmesh;
int numVert;
int level = 0;  //subdivision level
float scale = 1.0;
bool mouseLeft = false;
bool change = false;
int newX,newY,oldX,oldY;
float r_x , r_y;
float pitch = 45.0,yaw =-10.0;
float moveX=0.0,moveY = 0.0,moveZ = 0.0;
int control =0;
int cnewX,cnewY,coldX,coldY;
vector<Vertex> meshVertices;
float point_x,point_y;
int point_number = -1;
bool togle = false;
//---Reads data file and initializes drawing parameters ------------------
void initialise(char *filename)
{
	smesh = new Mesh();
	smesh->load_mesh(filename);
	smesh->triangulation();
	smesh->make_edges();
	//smesh->make_neighbours();
	//smesh->subdivide();
	cmesh = new Mesh();
	cmesh -> load_mesh(filename);
    		GLfloat front_light_position[] = { 0.0, 0.0, -10.0, 1.0 };
    		GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
    		GLfloat white_ambient[] = { 0.2, 0.2, 0.2, 1.0 }, white_diffuse[] = { 0.5, 0.5, 0.5, 1.0 }, white_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    		GLfloat white_shininess = 50.0;
    		glLightfv(GL_LIGHT2, GL_POSITION, front_light_position);
		glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);
		glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, white_shininess);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glEnable(GL_BLEND);
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
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);
		glRotatef(pitch, 0, 1, 0);
		glRotatef(yaw, 1, 0, 0);
		glScalef(scale,scale,scale);
		glTranslatef(moveX,moveY,moveZ);
		
		glEnable(GL_LIGHTING);
		if(control == 1)
		{
			togle = true;
			glColor3f(1.,0.,0.);
			cmesh->draw(control);
			meshVertices = cmesh->get_meshVertices();
			if(change){
				glPointSize(6);
				for(int i=0;i<meshVertices.size();i++)
					{
					if(point_number==i)
						{
						glPointSize(8);
						float x,y,z ;
						x=meshVertices[i].get_x();
						y=meshVertices[i].get_y();
						z=meshVertices[i].get_z();
						meshVertices[i].set_coords(x+point_x,y+point_y,z);
						cmesh->set_meshVertices(meshVertices);
						glBegin(GL_POINTS);
						glVertex3f(meshVertices[i].get_x(),meshVertices[i].get_y(),meshVertices[i].get_z());
						glEnd();
						}
					else
						{
						glPointSize(5);
						glBegin(GL_POINTS);
						glVertex3f(meshVertices[i].get_x(),meshVertices[i].get_y(),meshVertices[i].get_z());
						glEnd();
						}
					}
				}
			
		}
		else{
		glColor3f(1.,0.,0.);
		smesh->draw(control);}
		glFlush();
	}
	void mouse(int button, int state, int X, int Y)
	{
	if(change == false){
		newX = X;
		newY = Y;
	
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
			mouseLeft = true;
			}
		else
			{
			mouseLeft = false;
			}
		}
	if(change == true)
	{
		cnewX = X;
		cnewY = Y;
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
			mouseLeft = true;
			}
		else
			{
			mouseLeft = false;
			}
	}
		glutPostRedisplay();
}
void motion(int x,int y)
	{
		if(control == false){
			oldX = newX;
			oldY = newY;
			newX=x;
			newY=y;
			if(mouseLeft == true )
			{
			r_x = (newX - oldX) / (float) glutGet((GLenum) GLUT_WINDOW_WIDTH);
			r_y = (newY - oldY) / (float) glutGet((GLenum) GLUT_WINDOW_HEIGHT);
			pitch += (r_x * 360);
			yaw += (r_y * 360);
			} 
		}
		if(control == true && change == true)
		{
			coldX = cnewX;
			coldY = cnewY;
			cnewX=x;
			cnewY=y;
		
			 if(mouseLeft == true)
			{
			if((cnewX - coldX)<0)
			point_x = (point_x - 1.0)/ (float)( glutGet((GLenum) GLUT_WINDOW_WIDTH)*0.08) ;
			if((cnewX - coldX)>0)
			point_x = (point_x + 1.0)/ (float)( glutGet((GLenum) GLUT_WINDOW_WIDTH)*0.08) ;
			if((cnewY - coldY)<0)
			point_y = (point_y - 1.0 )/ (float)( glutGet((GLenum) GLUT_WINDOW_HEIGHT)*0.02) ;
			if((cnewY - coldY)>0)
			point_y = (point_y + 1.0)/ (float) (glutGet((GLenum) GLUT_WINDOW_HEIGHT)*0.02);
			}
		}
	glutPostRedisplay();
	
}
	//-- Callback function for processing keyboard events --------------------
void keyboard(unsigned char key, int x, int y)
	{
		if (key == ' ' && level < 4)
		{
			smesh->subdivide();
			level++;
		}
		if (key == '+')
		 scale *=2;
		if(key == '-')
		 scale /=2;
		if (key == 'a')
		moveX -= 1;
		if (key == 'd')
		moveX += 1;
		if (key == 's')
		moveY -= 1;
		if (key == 'w')
		moveY += 1;
		if (key == 'o')
		moveZ += 0.5;
		if (key == 'l')
		moveZ -= 0.5;
		if(key == 'c')
		{
			if (control == 1)
			{
			control = 0;
			vector<Vertex> cmeshVertices = cmesh->get_meshVertices();
			vector<Face> cmeshFaces = cmesh->get_meshFace();
			int cmesh_polygon_size = cmesh->get_polysize();
			smesh->modify(cmeshVertices,cmeshFaces,cmesh_polygon_size);
			for(int i=1;i<=level;i++)
				smesh->subdivide();
			}
			else
			{
			control = 1;
			}
		}
		if(key == 'v')
		change = change == false ? true : false;
		if(key == 'b' && change == true)
		point_number=(point_number+1)%meshVertices.size();
		
		glutPostRedisplay();
	}
	

	//-- Callback function for processing special keyboard events --------------
	
	//------- Main: Initialize glut window and register call backs -------------
	int main(int argc, char** argv)
	{
	   glutInit(&argc, argv);
	   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	   glutInitWindowSize (500, 500);
	   glutInitWindowPosition (10, 10);
	   glutCreateWindow ("Loop Subdivision");
	   initialise (argv[1]);

	   glutDisplayFunc(display);
	   glutKeyboardFunc(keyboard);
	   glutMouseFunc(mouse);
	   glutMotionFunc(motion);
	   glutMainLoop();
	   return 0;
	}
