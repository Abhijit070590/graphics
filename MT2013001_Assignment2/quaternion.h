/*
 * quaternion.h
 *
 *  Created on: 26-Jan-2014
 *      Author: abhijit
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include<cmath>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdlib.h>
#include"Vector.h"

class Quaternion{
private:
	float x;
	float y;
	float z;
	float w;
public:
	float getW() const {
		return w;
	}

	void setW(float w) {
		this->w = w;
	}

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
	Quaternion(float w,float x,float y,float z);
	
	Quaternion quaternion_conjugate();

};

GLdouble* quaternion_rotation_matrix(Quaternion q );
Vector quaternion_rotate_vector(Quaternion q, Vector v);
Quaternion make_rotation_quaternion_from_axis_and_angle(Vector axis, float angle);
Quaternion quaternion_multiply(Quaternion a,Quaternion b);
#endif /* QUATERNION_H_ */
