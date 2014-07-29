/*
 * quaternion.cpp
 *
 *  Created on: 28-Jan-2014
 *      Author: abhijit
 */
#include"../Header/quaternion.h"



 Quaternion::Quaternion(float w,float x,float y,float z)
 	{
		this->w=w;
 		this->x=x;
 		this->y=y;
 		this->z=z;
 		
 	}
 Quaternion make_rotation_quaternion_from_axis_and_angle(float angle, Vector axis) {
	Quaternion quat=Quaternion(
	cos(angle / 2.0),
	axis.getX() * sin(angle / 2.0),
	axis.getY() * sin(angle / 2.0),
	axis.getZ() * sin(angle / 2.0));
	return quat;
}

 Quaternion quaternion_multiply(Quaternion quat1, Quaternion quat2) {
	return (Quaternion( quat1.getW() * quat2.getW() - quat1.getX() * quat2.getX() - quat1.getY() * quat2.getY() - quat1.getZ() * quat2.getZ(),
		quat1.getW() * quat2.getX() + quat1.getX() * quat2.getW() + quat1.getY() * quat2.getZ() - quat1.getZ() * quat2.getY(), quat1.getY()
					* quat2.getW() + quat1.getZ() * quat2.getX() + quat1.getW() * quat2.getY() - quat1.getX() * quat2.getZ(), quat1.getW() * quat2.getZ() + quat1.getZ() * quat2.getW() + quat1.getX() * quat2.getY() - quat1.getY() * quat2.getX() )) ;
		}

 Quaternion Quaternion::quaternion_conjugate() {
	 Quaternion result = Quaternion(this->w,-(this->x),-(this->y),-(this->z));
	 return result;
 	 
 	}

 Vector quaternion_rotate_vector(Quaternion quat, Vector vect) {
			Quaternion tempQuat=Quaternion(
			0,
			vect.getX(),
			vect.getY(),
			vect.getZ());

			tempQuat = quaternion_multiply(quat, tempQuat);
			quat=quat.quaternion_conjugate();
			

			tempQuat = quaternion_multiply(tempQuat, quat);

			Vector temp = Vector ( tempQuat.getX(), tempQuat.getY(), tempQuat.getZ() );

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


