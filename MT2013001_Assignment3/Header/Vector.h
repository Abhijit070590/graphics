/*
 * Vector.h
 *
 *  Created on: 31-Jan-2014
 *      Author: abhijit
 */

#ifndef VECTOR_H_
#define VECTOR_H_
#include<cmath>

class Vector{
private:
	float x;
	float y;
	float z;

public:
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
	Vector(){ }
	Vector(float x,float y,float z);
	Vector scale(const int constant);
	Vector normalize() ;
	float norm();
	Vector & operator=(const Vector &v);
};

	Vector add(Vector &v1,Vector &v2);
	Vector sub(Vector &v1,Vector &v2);


	float dot_product(Vector a,Vector b);
	Vector cross_product(Vector a,Vector b);
#endif /* VECTOR_H_ */
