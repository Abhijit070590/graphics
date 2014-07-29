#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<GL/glut.h>
#include<cmath>
#include<cstring>
#include"quaternion.h"
using namespace std;
typedef struct _vertex
{
float x;
float y;
float z;
float confidence;
float intensity;
}Vertex;
typedef struct _face{
int v1;
int v2;
int v3;
}Face;
typedef struct _verNorm{
float x;
float y;
float z;
float n;
}VerNorm;
typedef struct _facNorm{
float x;
float y;
float z;
}FacNorm;

Vertex *vertices;
Face *faces;
VerNorm *vnorm;
int number_of_vertices,number_of_faces,flag2=0,lflag=0,shading=0;
FacNorm *fnorm;

int callback_set;
float previous_time, current_time;

//Scaling Variable
float zoom = 1;

//Sign variable for pitch value change
int op = 1;

//Angles
float yaw = 1;
float lightPitch = 0, pitch = 0;

float cameraDistance = 2;

//Mouse Button ref
int LeftButtonDown = 0;
int MiddleButtonDown = 0;
int ShiftLeftButtonDown = 0;

//Variables for Change in mouse position
int oldX, oldY, newX = 1, newY = 1;

//Variable for displacement of mouse
float moveX = 0, moveY = 0;

//variable to store maximum value of Z coordinate in given dataset
double maxZ = -100.0;

//Boolean variables
GLenum linePoly = GL_FALSE;
GLenum pointDraw = GL_FALSE;
GLenum backColor = GL_FALSE;
GLenum withLight = GL_FALSE;
GLint usePerVertexNormals = GL_TRUE;
GLint automaticRotate = GL_TRUE;
//Window size
GLint windW = 800, windH = 600;

int shade_model = GL_SMOOTH;

//String for storing light names
char *left_light, *right_light, *front_light, fileName[30];

enum {
			LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
		} LightValues;

		GLfloat leftLightX = -10.0;
		GLfloat leftLightZ = 10.0;
		GLfloat rightLightX = 10.0;
		GLfloat rightLightZ = -10.0;

		GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 }, green_light[] = { 0.0, 1.0, 0.0, 1.0 }, white_light[] = { 1.0, 1.0, 1.0, 1.0 };

		GLfloat left_light_position[] = { 10.0, 0.0, -5.0, 0.0 }, right_light_position[] = { -10.0, 0.0, -5.0, 1.0 }, front_light_position[] = { 0.0, 0.0, -10.0, 1.0 };

		GLfloat white_ambient[] = { 0.2, 0.2, 0.2, 1.0 }, white_diffuse[] = { 0.5, 0.5, 0.5, 1.0 }, white_specular[] = { 0.3, 0.3, 0.3, 1.0 }, white_shininess = 50.0;


void calculateNormal(int fCnt,Vertex v1,Vertex v2,Vertex v3)
{
	
	

	VerNorm va,vb,vr;

	va.x=v1.x-v2.x;
	va.y=v1.y-v2.y;
	va.z=v1.z-v2.z;

	vb.x=v1.x-v3.x;
	vb.y=v1.y-v3.y;
	vb.z=v1.z-v3.z;
	
	 vr.x = va.y * vb.z - vb.y * va.z;
  	 vr.y = vb.x * va.z - va.x * vb.z;
   	 vr.z = va.x * vb.y - vb.x * va.y;

	float val = sqrt( vr.x*vr.x + vr.y*vr.y + vr.z*vr.z );

	fnorm[fCnt].x = vr.x/val;
	fnorm[fCnt].y = vr.y/val;
	fnorm[fCnt].z = vr.z/val;
	
	vnorm[faces[fCnt].v1].x+=fnorm[fCnt].x;
	vnorm[faces[fCnt].v2].x+=fnorm[fCnt].x;
	vnorm[faces[fCnt].v3].x+=fnorm[fCnt].x;
	
	
	vnorm[faces[fCnt].v1].y+=fnorm[fCnt].y;
	vnorm[faces[fCnt].v2].y+=fnorm[fCnt].y;
 	vnorm[faces[fCnt].v3].y+=fnorm[fCnt].y;

	vnorm[faces[fCnt].v1].z+=fnorm[fCnt].z;
	vnorm[faces[fCnt].v2].z+=fnorm[fCnt].z;
	vnorm[faces[fCnt].v3].z+=fnorm[fCnt].z;
	
	vnorm[faces[fCnt].v1].n+=1.0;
	vnorm[faces[fCnt].v2].n+=1.0;
 	vnorm[faces[fCnt].v3].n+=1.0;
	
	
}
/**************************************************************************************************/
void nrmlinit()
{
	int m;
	for(m=0;m<number_of_vertices;m++)
	{
		vnorm[m].x=vnorm[m].y=vnorm[m].z=vnorm[m].z=0;
	}
}
void Rotation_by_quaternions(float angle,float x , float y,float z)
{
	Vector v =Vector(x,y,z);
	
	Quaternion q= make_rotation_quaternion_from_axis_and_angle(v,angle);
	glMultMatrixd(quaternion_rotation_matrix(q));
	}

/*******************************************************************************************************/
void load()
{
        int endHeader=0,endVertex=0;
        int temp=0;
        char str[300],s1[30],s2[30];
        FILE *file = fopen(fileName,"r");
        int i=0;
		int j=1;
		fseek(file,0,SEEK_END);
		long filesize=ftell(file);
//vertices=new Vertex[filesize];
		vnorm=new VerNorm[filesize];
//faces = new Face[filesize];
		fnorm = new FacNorm[filesize];
		fseek(file,0,SEEK_SET); 
        
	while (  strncmp( "element vertex", str,strlen("element vertex")) != 0  )
			{
				fgets(str,300,file);			// format
			}
	strcpy(str, str+strlen("element vertex"));
	sscanf(str,"%i", &number_of_vertices);
	vertices=new Vertex[filesize];
	
	fseek(file,0,SEEK_SET);
	while (  strncmp( "element face", str,strlen("element face")) != 0  )
			{
				fgets(str,300,file);			// format
			}
	strcpy(str, str+strlen("element face"));
	sscanf(str,"%i", &number_of_faces);
	faces = new Face[filesize];
	
	while (  strncmp( "end_header", str,strlen("end_header")) != 0  )
			{
				fgets(str,300,file);			// format
			}
	
	for(int i=0;i<number_of_vertices;i++)
		{
			fgets(str,300,file);
			sscanf(str,"%f %f %f %f %f",&vertices[i].x , &vertices[i].y ,  &vertices[i].z ,  &vertices[i].confidence  ,  &vertices[i].intensity);
		}
	
	for(int j=0;j<number_of_faces;j++)
		{
			fgets(str,300,file);
			sscanf(str,"%d %d %d %d ",&temp,&faces[j].v1,&faces[j].v2,&faces[j].v3);
			calculateNormal(j,vertices[faces[j].v1],vertices[faces[j].v2],vertices[faces[j].v3]);
		}
 
	cout << "load ok";
}

void Reshape(int width, int height) {
			windW = width;
			windH = height;
			glViewport(0, 0, windW, windH);
			// Set viewing frustum
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, (float) width / height, 0.001f, 80);

			// Modelview matrix reset
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		
void Render()
{
	
    	
    	if(usePerVertexNormals==0)
			for(int j=0;j<number_of_faces;j++)
			{
			glBegin(GL_TRIANGLES);
				glNormal3f(fnorm[j].x,fnorm[j].y,fnorm[j].z);
				glVertex3f(vertices[faces[j].v1].x,vertices[faces[j].v1].y,vertices[faces[j].v1].z);
				glVertex3f(vertices[faces[j].v2].x,vertices[faces[j].v2].y,vertices[faces[j].v2].z);
				glVertex3f(vertices[faces[j].v3].x,vertices[faces[j].v3].y,vertices[faces[j].v3].z);
			glEnd();
			}
		else
			for(int j=0;j<number_of_faces;j++)
			{
			glBegin(GL_TRIANGLES);
				glNormal3f((vnorm[faces[j].v1].x/vnorm[faces[j].v1].n),
					(vnorm[faces[j].v1].y/vnorm[faces[j].v1].n),
					(vnorm[faces[j].v1].z/vnorm[faces[j].v1].n));
				glVertex3f(vertices[faces[j].v1].x,vertices[faces[j].v1].y,vertices[faces[j].v1].z);
				glNormal3f((vnorm[faces[j].v2].x/vnorm[faces[j].v2].n),
					(vnorm[faces[j].v2].y/vnorm[faces[j].v2].n),
					(vnorm[faces[j].v2].z/vnorm[faces[j].v2].n));
				glVertex3f(vertices[faces[j].v2].x,vertices[faces[j].v2].y,vertices[faces[j].v2].z);
				glNormal3f((vnorm[faces[j].v3].x/vnorm[faces[j].v3].n),
					(vnorm[faces[j].v3].y/vnorm[faces[j].v3].n),
					(vnorm[faces[j].v3].z/vnorm[faces[j].v3].n));
				glVertex3f(vertices[faces[j].v3].x,vertices[faces[j].v3].y,vertices[faces[j].v3].z);
			glEnd();
			}
		//glFlush();
	
}    

void Draw() {

			double moveZ = 1;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_LIGHTING);
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			GLdouble rollSquare = leftLightZ * leftLightZ - lightPitch * lightPitch;
			if (rollSquare < 0)
				rollSquare *= -1;
			GLdouble roll = sqrt(rollSquare);

			if (withLight) {
				left_light_position[0] = lightPitch;
				left_light_position[2] = op * roll;
				glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
			}

			gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

			Rotation_by_quaternions(yaw / 10, 1, 0, 0);
			Rotation_by_quaternions(pitch / 10, 0, 1, 0);

			glTranslated(moveX, moveY, moveZ);

			glScalef(zoom, zoom, zoom);
			glEnable(GL_LIGHTING);
			Render();
			glPopMatrix();
			glutSwapBuffers();

		}
void animate(float dt) {

			if (automaticRotate) {
				if (lightPitch < op * leftLightZ)
					op = 1;
				else
					op = -1;

				lightPitch += (op * 0.065);
				pitch += (0.1);
				glutPostRedisplay();
			}
		}
		
void timer(int ignored) {

			current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			animate(current_time - previous_time);
			previous_time = current_time;
			glutTimerFunc(20, timer, -1);

		}
		
void visibility(int state) {
			if (state == GLUT_VISIBLE && !callback_set) {
				callback_set = 1;
				previous_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				timer(-1);
			}
		}
void Key(unsigned char key, int x, int y) {

			switch (key) {
			case '+':
				zoom /= 0.75;
				glutPostRedisplay();
				break;
			case '-':
				zoom *= 0.75;
				glutPostRedisplay();
				break;
			case 'l':
				withLight ^= 1;
				glutPostRedisplay();
				break;
			case 'n':
				usePerVertexNormals ^= 1;
				
				glutPostRedisplay();
				break;
			case 'w':
				linePoly = !linePoly;
				if (linePoly) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				} else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				glutPostRedisplay();
				break;
			case 'p':
				pointDraw = !pointDraw;
				if (pointDraw) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				} else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				glutPostRedisplay();
				break;
			case 'b':
				backColor = !backColor;
				if (backColor) {
					glClearColor(0.0, 0.0, 0.0, 1.0);
				} else {
					glClearColor(1.0, 1.0, 1.0, 1.0);
				}
				glutPostRedisplay();
				break;
			case 27:
				exit(0);
			}
		}

void mouse(int button, int state, int x, int y) {
			newX = x;
			newY = y;

			// Left mouse button pressed
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
				LeftButtonDown = 1;
				ShiftLeftButtonDown = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
			} else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
				MiddleButtonDown = 1;
			} else {
				MiddleButtonDown = ShiftLeftButtonDown = LeftButtonDown = 0;
			}
		}

		void motion(int x, int y) {
			float RelX, RelY;

			oldX = newX;
			oldY = newY;
			newX = x;
			newY = y;

			RelX = (newX - oldX) / (float) glutGet((GLenum) GLUT_WINDOW_WIDTH);
			RelY = (newY - oldY) / (float) glutGet((GLenum) GLUT_WINDOW_HEIGHT);

			if (ShiftLeftButtonDown) {
				// Change distance
				cameraDistance += 100 * RelY;

			} else if (LeftButtonDown) {
				// Change Rotation
				pitch += (RelX * 180);
				yaw += (RelY * 180);
			} else if (MiddleButtonDown) {
				// Translate
				moveX += RelX * 2;
				moveY -= RelY * 2;
			}

			glutPostRedisplay();
		}

		void selectLight(GLenum which, int value, char **label) {
			glEnable(which);
			switch (value) {
			case LIGHT_OFF:
				*label = "off";
				glDisable(which);
				break;
			case LIGHT_RED:
				*label = "red";
				glLightfv(which, GL_DIFFUSE, red_light);
				break;
			case LIGHT_WHITE:
				*label = "white";
				glLightfv(which, GL_DIFFUSE, white_light);
				break;
			case LIGHT_GREEN:
				*label = "green";
				glLightfv(which, GL_DIFFUSE, green_light);
				break;
			}
			glutPostRedisplay();
		}

		void shadingSelect(int value) {
			glShadeModel(value);
			glutPostRedisplay();
		}

		void normalsSelect(int value) {

			usePerVertexNormals = value;
			glutPostRedisplay();
		}

		void changeRotation(int value) {

			if (value == 0)
				automaticRotate = 0;
			if (value == 1)
				automaticRotate = 1;
			if (value == 2)
				withLight = 1;
			if (value == 3)
				withLight = 0;

		}

		void changeLeftLight(int value) {
			selectLight(GL_LIGHT0, value, &left_light);
		}

		void changeRightLight(int value) {
			selectLight(GL_LIGHT1, value, &right_light);
		}

		void changeFrontLight(int value) {
			switch (value) {
			case LIGHT_OFF:
				glDisable(GL_LIGHT2);
				break;
			case LIGHT_RED:
				glLightfv(GL_LIGHT2, GL_AMBIENT, red_light);
				break;
			case LIGHT_WHITE:
				glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);
				break;
			case LIGHT_GREEN:
				glLightfv(GL_LIGHT2, GL_AMBIENT, green_light);
				break;
			}
			glutPostRedisplay();
		}

void Init() {

			load();
			glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
			glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
			glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
			glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
			glLightfv(GL_LIGHT2, GL_POSITION, front_light_position);
			glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);
						glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
			glMaterialf(GL_FRONT, GL_SHININESS, white_shininess);

			glEnable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_NORMALIZE);
			glEnable(GL_BLEND);

		}

int main(int argc, char** argv){
	if (argc != 2) {
				printf("Provide ply filename as argument.\nEx: ./PlyRenderer bunny.ply %d", argc);
				return 0;
			} else
				strcpy(fileName, argv[1]);
	glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
			glutCreateWindow("Ply File Rendering");
			glutReshapeWindow(windW, windH);
			Init();
			glutReshapeFunc(Reshape);
			glutKeyboardFunc(Key);
			glutDisplayFunc(Draw);
			glutMouseFunc(mouse);
			glutMotionFunc(motion);
			glutVisibilityFunc(visibility);

			glutMainLoop();
			return 0;
		}
