#ifndef MESH_H
#define MESH_H
#include <vector>
using namespace std;

class Vertex
{private:
        float x;
        float y;
        float z;
        
public :
        float get_x(){return x;}
        float get_y(){return y;}
        float get_z(){return z;}
        void set_x(float _x){ x = _x;}
        void set_y(float _y){ y = _y;}
        void set_z(float _z){ z = _z;}
        vector<float> get_coords();
        void set_coords(float x1,float y1,float z1);
        bool operator==(Vertex v){
			if(x==v.get_x())
				if(y==v.get_y())
					if(z==v.get_z())
						return true;
					else
						return false;
				else
					return false;
			else
				return false;
        };
		bool operator!=(Vertex v){
			if(x == v.get_x() && y ==v.get_y() && z==v.get_z())
			return false;
			else
			return true;
		};      
};

class Face
{
private:
       vector<int> vertex_indices;
       vector<int> edge_indices;

public:
		void addVertexIndex(int i);
		vector<int> getVertexIndices();

	vector<int> getEdgeIndices() const {
		return edge_indices;
	}

	void addEdgeIndex(int edge_index) {
		edge_indices.push_back(edge_index);
	}
};

class Edge
{
private:
        vector<Vertex> vertices;
        vector<Vertex> opposites;

public :
		
        void set_vertex(Vertex v1,Vertex v2);
        vector<Vertex> get_vertex();
        void add_opposite(Vertex v);
        vector<Vertex> get_opposites();
        
        
};


class Mesh
{
private:
	vector<Vertex> vertices;
	vector<Edge> edges;
	vector<Face> faces;
	vector<vector<Vertex> > neighbours;
	
	unsigned int num_ver;
	unsigned int num_face;
	unsigned int num_edges;
	unsigned int polygon_size;
public:
	unsigned int get_num_ver();
	unsigned int get_num_face();
	unsigned int get_num_edges();
	void set_num_ver(unsigned int value);
	void set_num_edges(unsigned int value);
	void set_num_face(unsigned int value);
	void load_vertices(char* filename);
	void load_faces(char* filename);
	void load_mesh(char* filename);
	void triangulation();
	void make_edges();
	void draw(int controlmesh);
	int find_edge(Edge e);
	void make_neighbours();
	void normal(Vertex p1, Vertex p2, Vertex p3);
	int find_vertex(vector<Vertex> ee,vector<Vertex> ef);
	void subdivide();
	void modify(vector<Vertex> meshVertices,vector<Face> meshFaces,int mesh_polygon_size);
	vector<int> find_ad_vertex(int index,int face);
	vector<Vertex> get_neighbours(int pos)
	{
		return neighbours[pos];
	}
	vector<Vertex> get_meshVertices(){
	return vertices;
	}
	void set_meshVertices(vector<Vertex> target){
	vertices = target;
	}
	vector<Face> get_meshFace(){
	return faces;
	}
	void set_meshFace(vector<Face> target)
	{
	faces = target;
	}
	int get_polysize()
	{
	return polygon_size;
	}
	
	
};
#endif
