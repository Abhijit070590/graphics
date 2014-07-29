/*
 * beizeir.h
 *
 *  Created on: 18-Jan-2014
 *      Author: abhijit
 */
#ifndef BEIZEIR_H_
#define BEIZEIR_H_

#include<iostream>
#include<GL/glut.h>
#include<cmath>
#include<cstdlib>

class Point {
private :
	float x;
	float y;
	float z;
public :

	//function sets the values of x y z
	void setxyz(float x2, float y2, float z2);
	//operator overloading for '=' sign
    void assign(const Point &incomingPoint);


	float getX() const {
		return x;
	}

	void setX(float x) {
		this->x = x;
	}

	float getY() const {
		return y;
	}

	void setY(float y) {
		this->y = y;
	}

	float getZ() const {
		return z;
	}

	void setZ(float z) {
		this->z = z;
	}
};

class Mathematics{
	public :
	int factorial(int n);
	float Combination(float n,float k);
};

void myInit();
void myRenderer();
void myMouse(int button,int x,int y,int z);
void drawLine(Point p1, Point p2);
Point drawBeizier(Point PT[], double t);
Point deCastelJau(Point PT[],double t);
float getCasteljauPoint(float PT[],int i,int j,double t);
void keyboards(unsigned char key, int x, int y);

#endif /* BEIZEIR_H_ */
