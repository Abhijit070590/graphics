
#if !defined(H_MESH)
#define H_MESH

#include <vector>
#include <list>
#include <string>
using namespace std;

struct Polygon
{
	int vert[6];
	int vi[4];
};
struct Vertex;
struct Face;

struct Edge 
{
  Vertex* start_vertex; 
  Vertex* end_vertex;
  Face* face; 
  Edge* next; 
  Edge* prev; 
  Edge* twin_e;
};

struct Vertex 
{
  float x; 
  float y;
  float z;
  int index;
  Edge* edge;
};

struct Face 
{
  Edge* edge;
  int index;
};

class Mesh 
{
public:
    int _polySize;
	int _numVert, _numPoly;
	vector<Polygon*> _polygons;
    vector<Vertex*> _vertArray;
    list<Face*> _faceList;
    list<Edge*> _edgeList;
    vector<int> vertInd1, vertInd2, vertNewIndx;

	Mesh()
		: _polySize(0), _numVert(0), _numPoly(0)
	      {}
	~Mesh() {}
	int loadMeshOFF(const string& filename);
	int loadMeshOBJ(const string& filename);
	void loadMeshPLY(char* filename);
	void render();
	void triangulation();
	void renderBV(float& xmin, float& xmax,
		float& ymin, float& ymax, float& zmin, float& zmax);
	void setColorAmbientDiffuse(float colorR, float colorG, float colorB) const;
	void setColorSpecular(float colorR, float colorG, float colorB, float shininess) const;
    void boundingVolume(float& xmin, float& xmax,
		float& ymin, float& ymax, float& zmin, float& zmax);
	void normal(Vertex* p1, Vertex* p2, Vertex* p3) const;
	void createEdgeStruct();
	void createEdgePairs();
	vector<int> oneRingNbs(int vIndx) const;
	void subdivide();
	int search(int i1, int i2);
};

#endif