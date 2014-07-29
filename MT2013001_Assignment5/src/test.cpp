#include "../Headers/Mesh.h"
#include <iostream>

int main(int argc, char** argv)
{
	Mesh *s = new Mesh();
	s->loadMeshPLY(argv[1]);
	s->triangulate();
	std::cout<<"upto triangulation";
	s->createEdgeStruct();
	std::cout<<"upto subdivide";
	s->subdivide();
	return 0;
}

//if(nsize == 3)
//        {
//        	wt2 = 0.1875;
//        	wt1 = 0.4375;
//        }
//        else
//        {
//        	wt2 = 0.375 / (float)nsize;
//        	wt1 = 1.0 - (float)nsize * wt2;
//        }
//		vert1 = _vertArray[i];
//		sumx = wt1*(vert1->x);
//		sumy = wt1*(vert1->y);
//		sumz = wt1*(vert1->z);


//void Mesh::loadMeshPLY(char* filename)
//{
//	     int endHeader=0,endVertex=0;
//        int temp=0;
//        float x,y,z;
//        char str[300];
//        FILE *file = fopen(filename,"r");
//
//
//
//	while (  strncmp( "element vertex", str,strlen("element vertex")) != 0  )
//			{
//				fgets(str,300,file);			// format
//			}
//	strcpy(str, str+strlen("element vertex"));
//	sscanf(str,"%i", &_numVert);
//	cout<< "Number of vertices"<<_numVert<<endl;
//
//
//	while (  strncmp( "element face", str,strlen("element face")) != 0  )
//			{
//				fgets(str,300,file);			// format
//			}
//	strcpy(str, str+strlen("element face"));
//	sscanf(str,"%i", &_numPoly);
//	cout<< "Number of faces" << _numPoly<<endl;
//
//	do{
//		fgets(str,300,file);
//		cout << str << endl;
//	}while(strncmp( "end_header", str,strlen("end_header")) != 0  );
//	cout<<"Header ended";
//
//	for(int i=0;i<_numVert;i++)
//		{
//			fgets(str,300,file);
//			sscanf(str,"%f %f %f",&x,&y,&z);
//			Vertex *v = new Vertex;
//			v->x = x/_numVert;
//			v->y = y/_numVert;
//			v->z = z/_numVert;
//			v->index = i;
//			v->edge = NULL;
//
//			_vertArray.push_back(v);
//		}
//		cout <<"faces";
//		fgets(str,300,file);
//		sscanf(str,"%d " ,&temp);
//		_polySize = temp;
//		int i=0;
//
//			do{
//				Polygon* p  = new Polygon;
//			if(temp==3)
//				sscanf(str,"%d %d %d %d",&temp,&p->vert[0],&p->vert[1],&p->vert[2]);
//			if(temp==4)
//				sscanf(str,"%d %d %d %d %d",&temp,&p->vert[0],&p->vert[1],&p->vert[2],&p->vert[3]);
//			if(temp==5)
//				sscanf(str,"%d %d %d %d %d %d",&temp,&p->vert[0],&p->vert[1],&p->vert[2],&p->vert[3],&p->vert[4]);
//			printf("%d %d %d %d %d \n",temp,p->vert[0],p->vert[1],p->vert[2],p->vert[3]);
//			_polygons.push_back(p);
//			fgets(str,300,file);
//			i++;
//
//			}while(i<_numPoly);
//
//	fclose(file);
//	cout << "load ok";
//
//}
