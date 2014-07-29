#include "../Headers/Mesh1.h"
#include <iostream>
#include <fstream>
#include <GL/glut.h>
using namespace std;

int parseString(const char carr[]){
	int i=0, val=0;
	while(carr[i]!='\0'){
		if(carr[i]!=' ') val=val*10+(int)(carr[i]-48);
		i++;
	}
	return val;
}

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    unsigned int pos = txt.find( ch );
    unsigned int initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos + 1 ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

int Mesh::loadMeshOFF(const string& filename)
{
    int nedges, nvert;
    float vx, vy, vz;
    int ivert[4];

	ifstream ifile (filename.c_str(), ios::in);    //Type input file name 

	if(!ifile.is_open())
	{
        cout << "Error opening mesh file: " << filename << endl;
		return 0;
	}

	_vertArray.clear();
	_polygons.clear();
	ifile.ignore(500, '\n');				//ignore first line
    while(ifile.peek() == '#') ifile.ignore(590, '\n'); //ignore comments
	ifile >> _numVert >> _numPoly >> nedges;	// Read No. of vertices, polygons, edges

    for(int i = 0; i < _numVert; i++){                         //Read Vertex List 
		ifile >> vx >> vy >> vz;
		cout << vx << vy << vz;
        Vertex* vert = new Vertex;
		vert->x = vx;
		vert->y = vy;
		vert->z = vz;
		vert->index = i;
		vert->edge = NULL;
        _vertArray.push_back(vert);
	}

	for(int i=0; i<_numPoly; i++)
	{
		ifile >> nvert >> ivert[0] >> ivert[1] >> ivert[2];
        if ((_polytype == Mesh::QUAD && nvert != 4) ||
            (_polytype == Mesh::TRIANGLE && nvert != 3))
        {
            cout << "Polygon type and the number of vertices for each polygon do not match!" << endl;
            return 0;
        }
		if(_polytype == Mesh::QUAD) ifile>> ivert[3];
		else ivert[3] = 0;
		Polygon* poly = new Polygon();
		for(int j = 0; j < 4; j++) poly->vert[j] = ivert[j]; 
		_polygons.push_back(poly);
	}
	ifile.close();
    return 1;
}

int Mesh::loadMeshPLY(const string& filename)
{
	cout << "inside load";
	ifstream ifile (filename.c_str(), ios::in);    //Type input file name 
	int nedges, nvert;
    float vx, vy, vz;
    int ivert[4];
	
	if(!ifile.is_open())
	{
		cout << "Error opening mesh file " << filename << endl;
		return 0;
	}

    _numVert = 0;
    _numPoly = 0;
	_vertArray.clear();
	_polygons.clear();
	for( std::string line; getline( ifile, line ); )
	{
		string s1 = "element vertex";
		string s2 = "element face";
		string s3 = "end_header";
		cout << line;
		if(line.compare(0,s1.length()-1,s1)==0)
			{
				vector<string> s;
				split( line, s, ' ' );
				_numVert = parseString(s[2].c_str());
			}
		if(line.compare(0,s1.length()-1,s2)==0)
		{
			vector<string> s;
			split( line, s, ' ' );
			_numPoly = parseString(s[2].c_str());
		}
		if(line.compare(0,s1.length()-1,s3)==0)
		break;
	}
	for(int i=0;i<_numVert;i++)
	{
		ifile >> vx >> vy >> vz;
		Vertex* vert = new Vertex;
		vert->x = vx;
		vert->y = vy;
		vert->z = vz;
		cout << vx << vy << vz ;
		vert->index = i;
		vert->edge = NULL;
        _vertArray.push_back(vert);
	}
	for(int i=0; i<_numPoly; i++)
	{
		ifile >> nvert >> ivert[0] >> ivert[1] >> ivert[2];
        if ((_polytype == Mesh::QUAD && nvert != 4) ||
            (_polytype == Mesh::TRIANGLE && nvert != 3) )
        {
            cout << "Polygon type and the number of vertices for each polygon do not match!" << endl;
            return 0;
        }
		if(_polytype == Mesh::QUAD) ifile>> ivert[3];
		else ivert[3] = 0;
		Polygon* poly = new Polygon();
		for(int j = 0; j < 4; j++) poly->vert[j] = ivert[j]; 
		_polygons.push_back(poly);
	}
	ifile.close();
	
}

int Mesh::loadMeshOBJ(const string& filename)
{
    char c1, c2, carr[100];
    float vx, vy, vz;
    int ivert[4];

	ifstream ifile (filename.c_str(), ios::in);    //Type input file name 

	if(!ifile.is_open())
	{
		cout << "Error opening mesh file " << filename << endl;
		return 0;
	}

    _numVert = 0;
    _numPoly = 0;
	_vertArray.clear();
	_polygons.clear();
	while(ifile >> c1) {  //read the first command, skipping white space
		if(c1 == 'v'){
		    ifile.get(c2);    //the char following 'v' must be blank for vertex
			if(c2 == ' '){
				ifile >> vx >> vy >> vz;
                Vertex* vert = new Vertex;
		        vert->x = vx;
		        vert->y = vy;
		        vert->z = vz;
				vert->index = _numVert;
		        vert->edge = NULL;
                _vertArray.push_back(vert);
                _numVert ++;
			}
		}
		else if(c1 == 'f'){
			ifile.getline(carr, 100, '/');  //read first index
			ivert[0] = parseString(carr);
			ifile.ignore(100, ' ');  //ignore the remaining indices (texture, normal) before blank character

			ifile.getline(carr, 100, '/');
			ivert[1] = parseString(carr);
			ifile.ignore(100, ' ');

			ifile.getline(carr, 100, '/');
			ivert[2] = parseString(carr);
			if(_polytype == Mesh::QUAD)
			{
				ifile.ignore(100, ' ');

				ifile.getline(carr, 100, '/');
				ivert[3] = parseString(carr);
			}
			else ivert[3] = 0;
		    Polygon* poly = new Polygon();
			for(int j = 0; j < 4; j++) poly->vert[j] = ivert[j]; 
		    _polygons.push_back(poly);
            _numPoly ++;
		}
		ifile.ignore(900, '\n');
	}

	ifile.close();
    return 1;
}

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
