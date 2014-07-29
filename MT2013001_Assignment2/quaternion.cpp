/*
 * quaternion.cpp
 *
 *  Created on: 28-Jan-2014
 *      Author: abhijit
 */
#include"quaternion.h"



 Quaternion::Quaternion(float w,float x,float y,float z)
 	{
 		this->x=x;
 		this->y=y;
 		this->z=z;
 		this->w=w;
 	}
 Quaternion make_rotation_quaternion_from_axis_and_angle(Vector axis,float angle) {
 	  Quaternion result = Quaternion(cos(angle/2.0),
 	                      axis.getX()*sin(angle/2.0),
 	                      axis.getY()*sin(angle/2.0),
 	                      axis.getZ()*sin(angle/2.0)
 	                      );
 	  return result;
 	}
 Quaternion quaternion_multiply(Quaternion a,Quaternion b) {
 	  Quaternion result = Quaternion(a.getW()*b.getW() - a.getX()*b.getX() - a.getY()*b.getY() -a.getZ()*b.getZ(),
								a.getW()*b.getX() + a.getX()*b.getW() + a.getY()*b.getZ() - a.getZ()*b.getY(),
								a.getY()*b.getW() + a.getZ()*b.getX() + a.getW()*b.getY() - a.getX()*b.getZ(),
								a.getW()*b.getZ() + a.getZ()*b.getW() + a.getX()*b.getY() - a.getY()*b.getZ());
	return result; 	                      
 	}

 Quaternion Quaternion::quaternion_conjugate() {
	 Quaternion result = Quaternion(this->w,-(this ->x),-(this->y),-(this->z));
	 return result;
 	 
 	}

 Vector quaternion_rotate_vector(Quaternion q, Vector v) {
  	  
  	  
  	  Quaternion vq = Quaternion(0, v.getX(), v.getY(), v.getZ());
  	  vq = quaternion_multiply(q,vq);
  	  vq = quaternion_multiply(vq,q.quaternion_conjugate());
  	  Vector temp= Vector(vq.getX(), vq.getY(), vq.getZ());
  	  return temp;
  	}

 GLdouble* quaternion_rotation_matrix(Quaternion q ){
 	Vector axis[3],rotated_axis[3];
	GLdouble rotation_matrix[16];

 	axis[0]=Vector(1,0,0);
 	axis[1]= Vector(0,1,0);
 	axis[2]= Vector(0,0,1);


 	for(int i=0;i<3;i++)
 		rotated_axis[i]=quaternion_rotate_vector(q,axis[i]);

	int k=0;
 	for(int i=0;i<3;i++){
 		for(int j=0;j<3;j++){
 			rotation_matrix[k++]=dot_product(axis[j],rotated_axis[i]);
		}
		rotation_matrix[k++]=0;
	}
 	rotation_matrix[k++]=0;
 	rotation_matrix[k++]=0;
 	rotation_matrix[k++]=0;
 	rotation_matrix[k]=1;
	return rotation_matrix;
 }


