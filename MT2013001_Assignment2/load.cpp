#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<GL/glut.h>
#include<cmath>
#include<cstring>
//#include"quaternion.h"
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
float scale=4.0;
float rot1=0,rot2=0,rot3=0,rot4=0;
float xLight1=-50.0;
float tx=0.0,ty=0.0,tz=0.0,rx=0.0,ry=0.0,rz=0.0,angle=0.0,rox=0,roy=0;
float start1=0,start2=0,finish1=0,finish2=0;
int windW = 900, windH = 900;

float cameraDistance = 2;	


/******************************************Quaternion**********************************/

typedef struct Vector {
	float x, y, z;
} Vector;

//Structure to store quaternion
typedef struct Quaternion {
	float w, x, y, z;
} Quaternion;

// Dot product of two vectors
float dotProduct(Vector* vect1, Vector* vect2) {
	return (vect1->x * vect2->x + vect1->y * vect2->y + vect1->z * vect2->z);
}

//Create Quaternion using reference axis and angle of rotation
Quaternion createRotationQuaternion(float angle, Vector axis) {
	Quaternion quat;
	quat.w = cos(angle / 2.0);
	quat.x = axis.x * sin(angle / 2.0);
	quat.y = axis.y * sin(angle / 2.0);
	quat.z = axis.z * sin(angle / 2.0);
	return quat;
}

Quaternion multiplyQuaternion(Quaternion quat1, Quaternion quat2) {
	return (Quaternion ) { quat1.w * quat2.w - quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z,
		quat1.w * quat2.x + quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y, quat1.y
					* quat2.w + quat1.z * quat2.x + quat1.w * quat2.y - quat1.x * quat2.z, quat1.w * quat2.z + quat1.z * quat2.w + quat1.x * quat2.y - quat1.y * quat2.x } ;
		}

		Vector rotateVector(Quaternion quat, Vector* vect) {
			Quaternion tempQuat;
			tempQuat.w = 0;
			tempQuat.x = vect->x;
			tempQuat.y = vect->y;
			tempQuat.z = vect->z;

			tempQuat = multiplyQuaternion(quat, tempQuat);

			quat.x *= -1;
			quat.y *= -1;
			quat.z *= -1;

			tempQuat = multiplyQuaternion(tempQuat, quat);

			Vector temp = (Vector ) { tempQuat.x, tempQuat.y, tempQuat.z };

			return temp;
		}

		GLdouble* createRotationMatrix(Quaternion quat) {

			GLdouble rtMat[16];
			Vector axes[3];
			Vector rotateAbtAxes[3];

			axes[0].x = 1;
			axes[0].y = 0;
			axes[0].z = 0;
			axes[1].x = 0;
			axes[1].y = 1;
			axes[1].z = 0;
			axes[2].x = 0;
			axes[2].y = 0;
			axes[2].z = 1;

			rotateAbtAxes[0] = rotateVector(quat, &axes[0]);
			rotateAbtAxes[1] = rotateVector(quat, &axes[1]);
			rotateAbtAxes[2] = rotateVector(quat, &axes[2]);

			int i, j, count = 0;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					rtMat[count++] = dotProduct(&axes[j], &rotateAbtAxes[i]);
					//  printf("%f ",rtMat[count]);
				}
				rtMat[count++] = 0;
				//  printf("%f\n",rtMat[count]);
			}
			rtMat[count++] = 0;
//  printf("%f ",rtMat[count]);
			rtMat[count++] = 0;
			// printf("%f ",rtMat[count]);
			rtMat[count++] = 0;
			// printf("%f ",rtMat[count]);
			rtMat[count] = 1;
//  printf("%f\n",rtMat[count]);

			return rtMat;

		}

		void rotateUsingQuaternion(float angle, int aboutX, int aboutY, int aboutZ) {

			Quaternion quat = createRotationQuaternion(angle, (Vector ) { aboutX, aboutY, aboutZ });
			glMultMatrixd(createRotationMatrix(quat));

		}
/********************************************************************************************************/

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
/*******************************************************************************************************/
void load(char *filename)
{
        int endHeader=0,endVertex=0;
        int temp=0;
        char str[300],s1[30],s2[30];
        FILE *file = fopen(filename,"r");
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
/********************************************************************************************************/
void init() 
{
	if(flag2==0)
	{
  		 glDisable(GL_LIGHT1);
   		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   		GLfloat mat_shininess[] = { 50.0 };
   		GLfloat now[] ={0.5};
   		GLfloat light_position[] = { 1.0, 1.0, 1.0, -50.0 };
		 glClearColor (0.0, 0.0, 0.0, 0.0);
		   glShadeModel (GL_SMOOTH);
   		GLfloat cyan[] = {1.0f, 1.0f, 1.0f, 1.f};
   		glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  		 glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   		glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER,now);
   		glEnable(GL_LIGHTING);
   		glEnable(GL_LIGHT0);
   		glEnable(GL_DEPTH_TEST);
   		glEnable(GL_NORMALIZE);
  	}
 	 else
  	{
   		glDisable(GL_LIGHT0);
   		GLfloat light0Position[] = {1.0, 0.0, 1.0, 0.0}; 
		glLightfv(GL_LIGHT1, GL_POSITION, light0Position);
   		GLfloat ambient0[]={1.0, 1.0, 1.0, 1.0}; 
  		GLfloat diffuse0[]={1.0, 1.0, 1.0, 1.0};
  		 GLfloat specular0[]={1.0,1.0, 1.0, 1.0};
   		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient0);
   		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse0);
  		 glLightfv(GL_LIGHT1, GL_SPECULAR, specular0);
  		 glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient0);
  		 glEnable(GL_LIGHTING);
  		 glEnable(GL_LIGHT1);
  		 glEnable(GL_DEPTH_TEST);
  		 glEnable(GL_NORMALIZE);
  	 }

}
/********************************************************************************/
void moveLight(){
    if(xLight1>=-100.0&&lflag==0)
    {
    	xLight1-=10.0;
    	if(xLight1==-100.0)
    		lflag=1;
    }
    if(xLight1<=100.0&&lflag==1)
    {
    	xLight1+=10.0;
    	if(xLight1==100.0)
    		lflag=0;
    }
    xLight1=1;
    GLfloat light0Position[] = {1.0, 20.0, 1.0, -xLight1};
    glLightfv(GL_LIGHT1, GL_POSITION, light0Position);
}
/***************************************************************************************
void Rotation_by_quaternions(float angle,float x , float y,float z)
{
	Vector v =Vector(x,y,z);
	
	Quaternion q= make_rotation_quaternion_from_axis_and_angle(v,angle);
	glMultMatrixd(quaternion_rotation_matrix(q));
	}
/**************************************************************************************************/
void draw()
{
	
    	
    	if(shading==0)
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

void render()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
   	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   	//moveLight();
 	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   	gluPerspective(80,(float)(900/900) , 0.00001f, 90.0);
   	glPushMatrix();
   	glMatrixMode(GL_MODELVIEW);
   	
   	init();
   	glLoadIdentity();
   	glScalef(scale,scale,scale);
	rotateUsingQuaternion((start1-finish1)*180/10,1,0,0);
   	rotateUsingQuaternion((start2-finish2)*180/10,0,1,0);
   	
	//Rotation_by_quaternions((start2-finish2)*20/10,1,0,0);
	//Rotation_by_quaternions((start2-finish2)*20/10,0,1,0);
	/*glRotatef(angle/900,0,ry,0);
	glRotatef(angle/900,rx,0,0);
	glRotatef(rox,1,0,0);
	glRotatef(roy,0,1,0);*/
	glTranslatef(tx,ty,tz);
	
	//gluPerspective(80,(float)(900/900) , 0.00001f, 80.0);
	draw();
	glPopMatrix();
	glutSwapBuffers();
	}



/*******************************************************************************************************/
void keyboard(unsigned char x, int y,int z)
{
	//for changing the light
	if(x=='l')
	{
		if(flag2==0)
			flag2=1;
		 else
		 	flag2=0;
	}
	//for changing shading
	if(x=='s')
	{
		if(shading==0)
			shading=1;
		else
			shading=0;
	}
	if(x=='i')
	{
		scale=scale+0.4;
	}
	if(x=='o')
	{
		scale=scale-0.4;
	}
	if(x=='a')
	{
		tx-=0.003;
	}
	if(x=='d')
	{
		tx+=0.003;
	}
	
	if(x=='w')
	{
		ty+=0.003;
	}
	if(x=='x')
	{
		ty-=0.003;
	}
	if(x=='z')
	{
		tz+=0.003;
	}
	if(x=='c')
	{
		tz-=0.003;
	}
	glutPostRedisplay();
}
/*******************************************************************/
void mouse(int btn,int state,int x,int y)
{
	

	if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
	{
		start1=finish1=x;
		start2=finish2=y;
	}
	if(btn==GLUT_RIGHT_BUTTON&&state==GLUT_UP)
	{
		finish1=x;
		finish2=y;
		tx=tx+((finish1-start1)/900);
   		ty=ty+((start2-finish2)/900);
	}
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		rot1=rot3=x;
		rot2=rot4=y;
	}
	if(btn==GLUT_LEFT_BUTTON&&state==GLUT_UP)
	{
		rot3=x;
		rot4=y;
		if(rot3-rot1>0)
		{
			rx=1.0;
			angle+=(atan((rot4-rot2)/(rot3-rot1))+3.0);
		}
		else if(rot3-rot1<0)
		{
			rx=-1.0;
			angle-=(atan((rot4-rot2)/(rot3-rot1))+3.0);
		}
		if(rot4-rot2>0)
		{
			ry=1.0;
			angle+=(atan((rot4-rot2)/(rot3-rot1))+3.0);
		}
		else if(rot4-rot2<0)
		{
			ry=-1.0;
			angle-=(atan((rot4-rot2)/(rot3-rot1))+3.0);
		}
		
	}
	glutPostRedisplay();
}

void mouseRotate(int x,int y){
	rot1=x;
	rot2=y;
	glutPostRedisplay();
}

void moveMouse(int x,int y){
	const float  s=1;
	
	rox+=(rot1-x)/s;
	roy+=(rot2-y)/s;
	mouseRotate(x,y);
	glutPostRedisplay();
}
void Reshape(int w, int h) {
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

/***************************************************************************************************/
int main(int argc, char** argv)
{
	cout << "ok1";
	load("bunny.ply");
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
        glutInitWindowSize(900,900);
        glutInitWindowPosition(100,100);
       glutReshapeFunc(Reshape);
        glutCreateWindow("Bunny");
		glutMouseFunc(mouse);
		glutMotionFunc(moveMouse);
        glutKeyboardFunc(keyboard);
   	glutDisplayFunc(render);
   	glutMainLoop();    
	return 0;
}
