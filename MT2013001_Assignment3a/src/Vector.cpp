/*
 * Vector.cpp
 *
 *  Created on: 31-Jan-2014
 *      Author: abhijit
 */

#include"../Header/Vector.h"
Vector::Vector(float x,float y,float z){
	this->x=x;
	this->y=y;
	this->z=z;
	}
Vector & Vector::operator=(const Vector &v)
{
	this->x=v.getX();
	this->y=v.getY();
	this->z=v.getZ();
	}
 Vector add(Vector &v1,Vector &v2)
	{
	Vector result;
	result.setX((v1.getX()+v2.getX()));
	result.setY((v1.getY()+v2.getY()));
	result.setZ((v1.getZ()+v2.getZ()));
	return result;
	}

Vector sub(Vector &v1,Vector &v2)
{
	Vector result;
		result.setX((v1.getX()-v2.getX()));
		result.setY((v1.getY()-v2.getY()));
		result.setZ((v1.getZ()-v2.getZ()));
		return result;
}
 Vector Vector:: scale(const int constant)
 	{
		Vector v;
		v.setX(this->x*constant);
		v.setY(this->y*constant);
		v.setZ(this->z*constant);
		return v;
 		
 	}
 float Vector::norm(){
 		return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
 	}

 Vector Vector::normalize() {
	  return scale(1/norm());
	}

 float dot_product(Vector a, Vector b) {
 	  return (a.getX()*b.getX() + a.getY()*b.getY() + a.getZ()*b.getZ());
 	}
 Vector cross_product(Vector va,Vector vb) {
	 Vector Result=Vector(
	 va.getY() * vb.getZ() - vb.getY() * va.getZ(),
  	 vb.getX() * va.getZ() - va.getX() * vb.getZ(),
   	 va.getX() * vb.getY() - vb.getX() * va.getY());
	 return Result;
 	  //return (Vector){a.y*b.z - a.z*b.y, a.x*b.z - a.z*b.x, a.x*b.y - a.y*b.x};
 	}

