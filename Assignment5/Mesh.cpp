

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace std;

void Mesh::render()
{
	int nvert;
	list<Face*>::iterator iter;
	Vertex *vert;
	Face *face;
	Edge *edge;

	if(_polytype == Mesh::TRIANGLE)
	{
		glBegin(GL_TRIANGLES);
		nvert = 3;
	}
	else
	{
		glBegin(GL_QUADS);
		nvert = 4;
	}

	for(iter = _faceList.begin(); iter != _faceList.end(); iter++)
	{
		   face = *iter;
		   if(face == NULL) continue;
		   edge = face->edge;
		   normal(edge->vert, edge->next->vert, edge->next->next->vert);
		   for(int j = 0; j < nvert; j++)
		   {
			   vert = edge->vert;
			   glVertex3f(vert->x, vert->y,  vert->z); 
			   edge = edge->next;
		   }
	}
	glEnd();
}

void Mesh::renderBV(float& xmin, float& xmax,
		float& ymin, float& ymax, float& zmin, float& zmax) //Assumption: Triangle mesh
{
	float xv[3], yv[3], zv[3], xc, yc, zc;
	float xmn = xmin, xmx = xmax, ymn = ymin, ymx = ymax, zmn = zmin, zmx = zmax;  //updated BV parameters
	int iv;

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	for(int i = 0;  i < _polygons.size();  i++)
	{
		for(int j = 0;  j < 3;  j++)
		{
			iv = _polygons[i]->vert[j];
			xv[j] = _vertArray[iv]->x;
			yv[j] = _vertArray[iv]->y;
			zv[j] = _vertArray[iv]->z;
		}
		xc = (xv[0] + xv[1] + xv[2])/3.0;
		yc = (yv[0] + yv[1] + yv[2])/3.0;
		zc = (zv[0] + zv[1] + zv[2])/3.0;
		if(xc >= xmin && xc <= xmax && yc >= ymin && yc <= ymax &&
			zc >= zmin && zc <= zmax)
		{
			for(int j = 0; j < 3; j++)
			{
				if(xv[j] < xmn) xmn = xv[j];
				if(xv[j] > xmx) xmx = xv[j];
				if(yv[j] < ymn) ymn = yv[j];
				if(yv[j] > ymx) ymx = yv[j];
				if(zv[j] < zmn) zmn = zv[j];
				if(zv[j] > zmx) zmx = zv[j];
				glVertex3f(xv[j], yv[j], zv[j]);
			}
		}
	}
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(xmn, ymn, zmx); glVertex3f(xmx, ymn, zmx); glVertex3f(xmx, ymx, zmx); glVertex3f(xmn, ymx, zmx); 
	glVertex3f(xmx, ymn, zmx); glVertex3f(xmx, ymn, zmn); glVertex3f(xmx, ymx, zmn); glVertex3f(xmx, ymx, zmx); 
	glVertex3f(xmx, ymn, zmn); glVertex3f(xmn, ymn, zmn); glVertex3f(xmn, ymx, zmn); glVertex3f(xmx, ymx, zmn); 
	glVertex3f(xmn, ymn, zmn); glVertex3f(xmn, ymn, zmx); glVertex3f(xmn, ymx, zmx); glVertex3f(xmn, ymx, zmn); 
	glVertex3f(xmn, ymx, zmx); glVertex3f(xmx, ymx, zmx); glVertex3f(xmx, ymx, zmn); glVertex3f(xmn, ymx, zmn); 
	glVertex3f(xmx, ymn, zmx); glVertex3f(xmn, ymn, zmx); glVertex3f(xmn, ymn, zmn); glVertex3f(xmx, ymn, zmn); 
	glEnd();

	xmin = xmn; xmax = xmx; ymin = ymn; ymax = ymx; zmin = zmn; zmax = zmx;
}

void Mesh::setColorAmbientDiffuse(float colorR, float colorG, float colorB) const
{
	float col[4] = {colorR, colorG, colorB, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col);
}


void Mesh::setColorSpecular(float colorR, float colorG, float colorB, 
	                        float shininess) const
{
	float col[4] = {colorR, colorG, colorB, 1.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, col);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}


void Mesh::boundingVolume(float& xmin, float& xmax,
		float& ymin, float& ymax, float& zmin, float& zmax)
{
	vector<Vertex*>::iterator iter;
	float xv, yv, zv;

	for(iter = _vertArray.begin(); iter != _vertArray.end(); iter++)
	{
		xv = (*iter)->x;   yv = (*iter)->y;   zv = (*iter)->z;
		if(iter == _vertArray.begin())
		{
			xmin = xv;   xmax = xv;
			ymin = yv;   ymax = yv;
			zmin = zv;   zmax = zv;
		}
		else
		{
			if(xmin > xv)      xmin = xv;
			else if(xmax < xv) xmax = xv;
			if(ymin > yv)      ymin = yv;
			else if(ymax < yv) ymax = yv;
			if(zmin > zv)      zmin = zv;
			else if(zmax < zv) zmax = zv;
		}
	}
}

void Mesh::normal(Vertex* p1, Vertex* p2, Vertex* p3) const
{
	float nx, ny, nz;
	nx = (p1->y)*((p2->z)-(p3->z))+(p2->y)*((p3->z)-(p1->z))+(p3->y)*((p1->z)-(p2->z));
	ny = (p1->z)*((p2->x)-(p3->x))+(p2->z)*((p3->x)-(p1->x))+(p3->z)*((p1->x)-(p2->x));
	nz = (p1->x)*((p2->y)-(p3->y))+(p2->x)*((p3->y)-(p1->y))+(p3->x)*((p1->y)-(p2->y));
	glNormal3f(nx, ny, nz);
}


void Mesh::createEdgeStruct()
{
	int numEdges, ivert;
	if(_numVert == 0 || _numPoly == 0) return;
	_faceList.clear();
	_edgeList.clear();
	if(_polytype == Mesh::TRIANGLE) numEdges = 3;
	else  numEdges = 4;
	
	Edge *edge, *eprev, *first;
	Vertex *vrtx;

	for(int i = 0; i < _numPoly; i++)
	{ 
		Face *face = new Face;   //One face per polygon
		for(int j = 0; j < numEdges; j++)
		{
			edge = new Edge;
			ivert = _polygons[i]->vert[j];
			vrtx = _vertArray[ivert];
			edge->vert = vrtx;
			if(vrtx->edge == NULL) vrtx->edge = edge;
			edge->face = face;
			edge->pair = NULL;
			if(j > 0)
			{
				edge->prev = eprev;
				eprev->next = edge;
			}
			else first = edge;
			eprev = edge;
			_edgeList.push_back(edge);
		}
		first->prev = edge;
		edge->next = first;
		face->edge = edge;
        face->index = i;
		_faceList.push_back(face);
	}

	createEdgePairs();
	glEnable(GL_NORMALIZE);
}


void Mesh::createEdgePairs()
{
	list<Edge*>::iterator iter1, iter2;
	Edge *edge, *epair;
	
	for(iter1 = _edgeList.begin(); iter1 != _edgeList.end(); iter1++)
	{
		edge = *iter1;
		if(edge->pair == NULL)
		{
			for(iter2 = iter1; iter2 != _edgeList.end(); iter2++)     //search for pair
			{
				epair = *iter2;
				if(epair->vert == edge->prev->vert && epair->prev->vert == edge->vert){
					edge->pair = epair;
					epair->pair = edge;
					break;
				} //if
			} //for iter2
		} //if
	}//for iter1
}

void Mesh::loadMeshPLY(char* filename)
{
	     int endHeader=0,endVertex=0;
        int temp=0;
        float x,y,z;
        char str[300];
        FILE *file = fopen(filename,"r");
		
		
        
	while (  strncmp( "element vertex", str,strlen("element vertex")) != 0  )
			{
				fgets(str,300,file);			// format
			}
	strcpy(str, str+strlen("element vertex"));
	sscanf(str,"%i", &_numVert);
	cout<< "Number of vertices"<<_numVert<<endl;
	
	
	while (  strncmp( "element face", str,strlen("element face")) != 0  )
			{
				fgets(str,300,file);			// format
			}
	strcpy(str, str+strlen("element face"));
	sscanf(str,"%i", &_numPoly);
	cout<< "Number of faces" << _numPoly<<endl;
	
	do{
		fgets(str,300,file);
		cout << str << endl;
	}while(strncmp( "end_header", str,strlen("end_header")) != 0  );
	cout<<"Header ended";

	for(int i=0;i<_numVert;i++)
		{
			fgets(str,300,file);
			sscanf(str,"%f %f %f",&x,&y,&z);
			Vertex *v = new Vertex;
			v->x = x/_numVert;
			v->y = y/_numVert;
			v->z = z/_numVert;
			v->index = i;
			v->edge = NULL;

			_vertArray.push_back(v);
		}
		cout <<"faces";
		fgets(str,300,file);
		sscanf(str,"%d " ,&temp);
		
		int i=0;

			do{
				Polygon* p  = new Polygon;
			if(temp==3)
				sscanf(str,"%d %d %d %d",&temp,&p->vert[0],&p->vert[1],&p->vert[2]);
			if(temp==4)
				sscanf(str,"%d %d %d %d %d",&temp,&p->vert[0],&p->vert[1],&p->vert[2],&p->vert[3]);
			if(temp==5)
				sscanf(str,"%d %d %d %d %d %d",&temp,&p->vert[0],&p->vert[1],&p->vert[2],&p->vert[3],&p->vert[4]);
			printf("%d %d %d %d %d \n",temp,p->vert[0],p->vert[1],p->vert[2],p->vert[3]);
			_polygons.push_back(p);
			fgets(str,300,file);
			i++;

			}while(i<_numPoly);

	fclose(file);
	cout << "load ok";
	
}

void Mesh::triangulation()
{
	vector<Polygon*> newPoly;
	for(int i=0;i<_numPoly;i++)
		{
			Polygon *p1 = new Polygon;
			Polygon *p2 = new Polygon;

			p1->vert[0] = _polygons[i]->vert[0];
			p1->vert[1] = _polygons[i]->vert[1];
			p1->vert[2] = _polygons[i]->vert[2];

			p2->vert[0] = _polygons[i]->vert[0];
			p2->vert[1] = _polygons[i]->vert[2];
			p2->vert[2] = _polygons[i]->vert[3];

			newPoly.push_back(p1);
			newPoly.push_back(p2);
		}
		_polygons.clear();
		_polygons =newPoly;
		_numPoly = newPoly.size();
}
vector<int> Mesh::oneRingNbs(int vIndx) const
{
	vector<int> indices;
	//Listing 8.5
    Vertex* p = _vertArray[vIndx];
    Edge *e0 = p->edge;
	Edge *edge = e0;
	Edge *pair;
	do
	{
		pair = edge->pair;
		indices.push_back(pair->vert->index);
		edge = pair->prev;
	}while (edge != e0);
	return indices;
}
