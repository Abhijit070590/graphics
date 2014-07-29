

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>
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

	if(_polySize == 3)
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
		   normal(edge->start_vertex, edge->next->start_vertex, edge->next->next->start_vertex);
		   for(int j = 0; j < nvert; j++)
		   {
			   vert = edge->start_vertex;
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
	int numEdges, ivert,ivert2;
	if(_numVert == 0 || _numPoly == 0) return;
	_faceList.clear();
	_edgeList.clear();
	if(_polySize == 3) numEdges = 3;
	else  numEdges = 4;
	
	Edge *edge, *eprev, *first;
	Vertex *vrtx,*vrtx2;

	for(int i = 0; i < _numPoly; i++)
	{ 
		Face *face = new Face;   //One face per polygon
		for(int j = 0; j < numEdges; j++)
		{
			edge = new Edge;
			ivert = _polygons[i]->vert[j];
			ivert2 = _polygons[i]->vert[(j+1)%_polySize];
			vrtx = _vertArray[ivert];
			edge->start_vertex = vrtx;
			if(vrtx->edge == NULL) vrtx->edge = edge;
			edge->face = face;
			edge->twin_e = NULL;
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
		if(edge->twin_e == NULL)
		{
			for(iter2 = iter1; iter2 != _edgeList.end(); iter2++)     //search for pair
			{
				epair = *iter2;
				if(epair->start_vertex == edge->prev->start_vertex && epair->prev->start_vertex == edge->start_vertex){
					edge->twin_e = epair;
					epair->twin_e = edge;
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
		_polySize = temp;
		cout << "_polySize"<<_polySize<<endl;
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
	if(_polySize==4){
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
	}
	if(_polySize==4)
	{
		for(int i=0;i<_numPoly;i++)
		{
			Polygon *p1 = new Polygon;
			Polygon *p2 = new Polygon;
			Polygon *p3 = new Polygon;

			p1->vert[0] = _polygons[i]->vert[0];
			p1->vert[1] = _polygons[i]->vert[1];
			p1->vert[2] = _polygons[i]->vert[2];

			p2->vert[0] = _polygons[i]->vert[0];
			p2->vert[1] = _polygons[i]->vert[2];
			p2->vert[2] = _polygons[i]->vert[3];

			p3->vert[0] = _polygons[i]->vert[0];
			p3->vert[1] = _polygons[i]->vert[2];
			p3->vert[2] = _polygons[i]->vert[3];

			newPoly.push_back(p1);
			newPoly.push_back(p2);
		}
	}
		_polygons.clear();
		_polySize = 3;
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
		pair = edge->twin_e;
		indices.push_back(pair->start_vertex->index);
		edge = pair->prev;
	}while (edge != e0);
	return indices;
}
int Mesh::search(int i1, int i2)
{
    int size = vertInd1.size();
    if(size == 0) return -1;
    for(int i = 0;  i < size; i++)
    {
        if(vertInd1[i] == i2  && vertInd2[i] == i1) return vertNewIndx[i];
    }
    return -1;
}

void Mesh::subdivide()
{
    int nsize;
    int numVert;
    vector<Vertex*> vertNewArray;  //Temporary array holding transformed and new vertices
    vector<Polygon*> faceNewArray;
    vector<Polygon*> faceOldArray;

	Vertex *vert1, *vert2, *vert3, *vert4, *vertNew;
	Edge *edge;
	vector<int> neighbs;
    numVert = _vertArray.size();
    float wt1 = 0.625, wt2;
	float sumx, sumy, sumz;
	//Update existing vertices  --- Eq. (8.20) ----
    for(int i = 0;  i < numVert; i++)
    {
        neighbs = oneRingNbs(i);
        nsize = neighbs.size();
        if(nsize == 3)
        {
       	wt2 = 0.1875;
       	wt1 = 0.4375;
       }
        else
        {
       	wt2 = 0.375 / (float)nsize;
       	wt1 = 1.0 - (float)nsize * wt2;
        }
		vert1 = _vertArray[i];
		sumx = wt1*(vert1->x);
		sumy = wt1*(vert1->y);
		sumz = wt1*(vert1->z);
		for(int k = 0;  k < nsize; k++)
		{
			vert2 = _vertArray[neighbs[k]];
			sumx += wt2*(vert2->x);
			sumy += wt2*(vert2->y);
			sumz += wt2*(vert2->z);
		}
		vertNew = new Vertex;
		vertNew->x = sumx;  vertNew->y = sumy;  vertNew->z = sumz;
		vertNew->index = i;  vertNew->edge = vert1->edge;
		vertNewArray.push_back(vertNew);
        neighbs.clear();
    }
	
	int vsize = numVert;
	vertInd1.clear();
	vertInd2.clear();
	vertNewIndx.clear();
	Face *face;
    Polygon *fnew, *fold;
    list<Face*>::iterator f_iter;
	int vindx;  //edge vertex indices
    //Create new vertices
	for(f_iter = _faceList.begin(); f_iter != _faceList.end(); f_iter++)
	{
		face = *f_iter;
		edge = face->edge;
        fnew = new Polygon;
		fold = new Polygon;
		for(int k = 0;  k < 3;  k++)   //Assumption: Triangular mesh
		{
			vert1 = edge->start_vertex;
			fold->vi[k] = vert1->index;
			vert2 = edge->twin_e->start_vertex;
            vindx = search(vert1->index,  vert2->index);
			if( vindx < 0)  //create a new vertex
			{
				vertInd1.push_back(vert1->index);
				vertInd2.push_back(vert2->index);
                vertNewIndx.push_back(vsize);
				vertNew = new Vertex;
				vert3 = edge->next->start_vertex;
				vert4 = edge->twin_e->next->start_vertex;
                //--Eq. (8.19)--
				vertNew->x = 0.375*((vert1->x)+(vert2->x)) + 0.125*((vert3->x)+(vert4->x));
				vertNew->y = 0.375*((vert1->y)+(vert2->y)) + 0.125*((vert3->y)+(vert4->y));
				vertNew->z = 0.375*((vert1->z)+(vert2->z)) + 0.125*((vert3->z)+(vert4->z));
				vertNew->index = vsize;
                fnew->vi[k] = vsize;
				vertNew->edge = edge;
				vertNewArray.push_back(vertNew);
				vsize++;
			}
            else   fnew->vi[k] = vindx;   //new vertex already created
			edge = edge->next;
		}
        faceNewArray.push_back(fnew);
		faceOldArray.push_back(fold);
	}

    //Restructure and retriangulate each face
	Edge *e[3], *enew1, *enew2, *enew3;
	Face *f[3];
	Vertex *vn[3], *vo[3];
	int k1, k2;

	int fsize = _faceList.size();
	f_iter = _faceList.begin();
	for(int findx = 0;  findx < fsize; findx++)
	{
		f[0] = *f_iter;    //current face
		f[1] = new Face; //Create 2 more sub-faces
		f[2] = new Face;
		e[0] = f[0]->edge;
		e[1] = e[0]->next;
		e[2] = e[1]->next;
        fnew = faceNewArray[findx];
		fold = faceOldArray[findx];
		for(int k = 0;  k < 3;  k++)  
		{
            vindx = fnew->vi[k];
            vn[k] = vertNewArray[vindx];
			vindx = fold->vi[k];
			vo[k] = vertNewArray[vindx];
		}
		for(int k = 0;  k < 3;  k++)  
		{
			enew1 = new Edge;  //Create 2 new edges for each sub-face
			enew2 = new Edge;
            e[k]->start_vertex = vn[k]; e[k]->face = f[k];
			e[k]->next = enew1; e[k]->prev = enew2;
			k1 = (k+2)%3;
			enew1->start_vertex = vn[k1]; enew1->prev = e[k]; enew1->next = enew2; enew1->face = f[k];
			enew2->start_vertex = vo[k1]; enew2->prev = enew1; enew2->next = e[k]; enew2->face = f[k];
			f[k]->edge = e[k];
			vo[k1]->edge = enew2;
			_edgeList.push_back(enew1);
			_edgeList.push_back(enew2);
		}
		_faceList.push_back(f[1]);
		_faceList.push_back(f[2]);
		//Create middle face
		face = new Face;
		enew1 = new Edge;  //Create 3 new edges for each sub-face
		enew2 = new Edge;
		enew3 = new Edge;
		enew1->start_vertex = vn[0]; enew1->prev = enew3; enew1->next = enew2; enew1->face = face;
		enew2->start_vertex = vn[1]; enew2->prev = enew1; enew2->next = enew3; enew2->face = face;
		enew3->start_vertex = vn[2]; enew3->prev = enew2; enew3->next = enew1; enew3->face = face;
		_edgeList.push_back(enew1);
		_edgeList.push_back(enew2);
		_edgeList.push_back(enew3);
		face->edge = enew1;
		_faceList.push_back(face);
		f_iter++;
	}
	_vertArray.clear();
	for(int i = 0;  i < vsize; i++) _vertArray.push_back(vertNewArray[i]);

	list<Edge*>::iterator e_iter;
	for(e_iter = _edgeList.begin(); e_iter != _edgeList.end(); e_iter++) (*e_iter)->twin_e = NULL;

    _numVert = vsize;
	vertNewArray.clear();
	faceNewArray.clear();
	faceOldArray.clear();
	createEdgePairs();
}