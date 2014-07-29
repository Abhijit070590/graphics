/*
 * beizeir.cpp
 *
 *  Created on: 18-Jan-2014
 *      Author: abhijit
 */

#include "beizeir.h"
using namespace std;
Point abc[1000];
int SCREEN_HEIGHT = 500;
int points = 0;
int clicks = 0;
int choice = -1;
bool rotate=false;
bool translate=false;
bool scale=false;
bool reset = false;

void Point::setxyz(float x2, float y2, float z2) {
	x = x2;
	y = y2;
	z = z2;
}

void Point::assign(const Point &incomingPoint) {
	x = incomingPoint.x;
	y = incomingPoint.y;
	z = incomingPoint.z;

}

int Mathematics::factorial(int n) {

	if (n <= 1)
		return 1;
	else
		n = n * factorial(n - 1);
	return n;

}

float Mathematics::Combination(float n, float k) {
	float ans;
	ans = factorial(n) / (factorial(k) * factorial(n - k));
	return ans;
}

void drawDot(float x, float y, float z) {
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
	glFlush();
}

void myRenderer() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();

}

void myMouse(int button, int state, int x, int y) {
	// If left button was clicked
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Store where mouse was clicked, Y is backwards.
		abc[points].setxyz((float) x, (float) (SCREEN_HEIGHT - y), 0.0);
		points++;
		if (reset == true) {
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
			if(rotate)
					glRotatef(5,0,0,1);
			if(scale)
				glScalef(1.1,1.1,1.1);
			if(translate)
				glTranslatef(1,0,0);
			for (int i = 0; i < points; i++)
				drawDot(abc[i].getX(), abc[i].getY(), 0.0);
		}
		// Draw the red  dot.
		else
		{

			drawDot(x, SCREEN_HEIGHT - y, 0.0);
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && choice == 0)

	{
		clicks = points;
		glColor3f(0.2, 1.0, 0.0);
		// Drawing the control lines
		for (int k = 0; k < clicks - 1; k++)
			drawLine(abc[k], abc[k + 1]);

		Point p1 = abc[0];
		/* Draw each segment of the curve.Make t increment in smaller amounts for a more detailed curve.*/
		for (double t = 0.0; t <= 1.0; t = t + 0.01) {
			Point p2;
			p2.assign(drawBeizier(abc, t));
			drawLine(p1, p2);
			p1.assign(p2);
		}
		glColor3f(0.0, 0.0, 0.0);
		reset = true;
		//points = 0;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && choice == 1)

	{
		clicks = points;
		glColor3f(0.2, 1.0, 0.0);
		// Drawing the control lines
		for (int k = 0; k < clicks - 1; k++)
			drawLine(abc[k], abc[k + 1]);

		Point p1 = abc[0];
		/* Draw each segment of the curve.Make t increment in smaller amounts for a more detailed curve.*/
		for (double t = 0.0; t <= 1.0; t = t + 0.01) {
			Point p2;
			p2.assign(deCastelJau(abc, t));
			drawLine(p1, p2);
			p1.assign(p2);
		}
		glColor3f(0.0, 0.0, 0.0);
		reset = true;
		//points = 0;
	}
	//}
}
void myInit() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 500.0);

}

Point drawBeizier(Point PT[], double t) {
	Point P;
	float x = 0.0, y = 0.0, z = 0.0;
	Mathematics m;
	x = pow((1 - t), (float) (clicks - 1)) * PT[0].getX();
	y = pow((1 - t), (float) (clicks - 1)) * PT[0].getY();
	z = pow((1 - t), (float) (clicks - 1)) * PT[0].getZ();
	for (int i = 1; i < clicks - 1; i++) {
		x = x
				+ m.Combination((float) clicks - 1, (float) i)
						* pow(t, (double) i)
						* pow((1 - t), (double) (clicks - i - 1))
						* PT[i].getX();
		y = y
				+ m.Combination((float) clicks - 1, (float) i)
						* pow(t, (double) i)
						* pow((1 - t), (double) (clicks - i - 1))
						* PT[i].getY();
		z = z
				+ m.Combination((float) clicks - 1, (float) i)
						* pow(t, (double) i)
						* pow((1 - t), (double) (clicks - i - 1))
						* PT[i].getZ();
	}
	x = x + pow(t, (float) (clicks - 1)) * PT[clicks - 1].getX();
	y = y + pow(t, (float) (clicks - 1)) * PT[clicks - 1].getY();
	z = z + pow(t, (float) (clicks - 1)) * PT[clicks - 1].getZ();
	P.setX(x);
	P.setY(y);
	P.setZ(z);
	return P;
}
Point deCastelJau(Point PT[], double t) {
	float x[1000];
	float y[1000];
	float z[1000];
	for (int i = 0; i < points; i++) {
		x[i] = PT[i].getX();
		y[i] = PT[i].getY();
		z[i] = PT[i].getZ();
	}
	Point temp;
	temp.setX(getCasteljauPoint(x, 0, points, t));
	temp.setY(getCasteljauPoint(y, 0, points, t));
	temp.setZ(getCasteljauPoint(z, 0, points, t));
	return temp;

}

float getCasteljauPoint(float PT[], int i, int size, double t) {
	float Q[size];
	for(int i=0;i<size;i++)
		Q[i]=PT[i];
	for(int k=1;k<size;k++)
		for(int n=0;n<size-k;n++)
			Q[n]=(1-t)*Q[n] + t*Q[n+1];
	return Q[0];
}
void drawLine(Point p1, Point p2) {
	glBegin(GL_LINES);
	glVertex3f(p1.getX(), p1.getY(), p1.getZ());
	glVertex3f(p2.getX(), p2.getY(), p2.getZ());
	glEnd();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	if(key=='b')
		choice=0;
	if(key=='d')
		choice=1;
	if(key=='r')
		rotate=true;
	if(key=='s')
		scale=true;
	if(key=='t')
		translate=true;
}
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //since we would be using a line only a GLUT_SINGLE-single buffer window GLUT_RGB because color is required
	glutInitWindowSize(640, 500); //window size
	glutInitWindowPosition(100, 150); //window position
	glutCreateWindow("Beizier Curve"); //window title
	glutMouseFunc(myMouse);
	glutDisplayFunc(myRenderer);
	glutKeyboardFunc(keyboard);
	myInit();
	glutMainLoop();

	return 0;
}
