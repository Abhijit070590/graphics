#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "../Header/Mesh.h"
using namespace std;
void Vertex::set_coords(float x1,float y1,float z1)
{
        x=x1;
        y=y1;
        z=z1;
}
vector<float> Vertex::get_coords()
{
        vector<float> coords;
        coords.push_back(x);
        coords.push_back(y);
        coords.push_back(z);
        return coords;
}



void Edge::set_vertex(Vertex v1,Vertex v2)
{
	vertices.push_back(v1);
	vertices.push_back(v2);
}

vector<Vertex> Edge::get_vertex()
{
	return vertices;
}

void Edge::add_opposite(Vertex v)
{
	opposites.push_back(v);

}

vector<Vertex> Edge::get_opposites(){
	return opposites;
}

void Face::addVertexIndex(int index)
{
	vertex_indices.push_back(index);

}

vector<int> Face::getVertexIndices(){
	
	return vertex_indices;
}

unsigned int Mesh::get_num_ver()
{
	return num_ver;
}
unsigned int Mesh::get_num_face()
{
	return num_face;
}

unsigned int Mesh::get_num_edges()
{
	return num_edges;
}

void Mesh::set_num_ver(unsigned int value)
{
	num_ver = value;
}

void Mesh::set_num_edges(unsigned int value)
{
	num_edges=value;

}

void Mesh::set_num_face(unsigned int value)
{
	num_face = value;

}

void Mesh::load_mesh(char* filename)
{
	  FILE *file = fopen(filename,"r");
	  char str[300];
	  unsigned int num;
	  while (  strncmp( "element vertex", str,strlen("element vertex")) != 0  )
	  			{
	  				fgets(str,300,file);
	  			}
	  strcpy(str, str+strlen("element vertex"));
	  sscanf(str,"%i", &num);
	  set_num_ver(num);
	  while (  strncmp( "element face", str,strlen("element face")) != 0  )
	  			{
	  				fgets(str,300,file);			// format
	  			}
	  strcpy(str, str+strlen("element face"));
	  sscanf(str,"%i", &num);
	  set_num_face(num);
	  while (  strncmp( "end_header", str,strlen("end_header")) != 0  )
	  			{
	  				fgets(str,300,file);			// format
	  			}
	  float x1=0.0,y1=0.0,z1=0.0;
	  for(int i=0;i<num_ver;i++)
	  		{
	  			fgets(str,300,file);
	  			Vertex temp;
	  			float x,y,z;
	  			sscanf(str,"%f %f %f",&x , &y , &z);
	  			x1+=x; y1+=y; z1+=z;
	  			temp.set_coords(x,y,z);
	  			vertices.push_back(temp);
	  		}
	  x1=x1/num_ver;
	  y1=y1/num_ver;
	  z1=z1/num_ver;
	  for(int i=0;i<num_ver;i++)
	  {
	  Vertex v1 = vertices[i];
	  v1.set_x(v1.get_x()-x1);
	  v1.set_y(v1.get_y()-y1);
	  v1.set_z(v1.get_z()-z1);
	  vertices[i]=v1;
	  }
	  int point = 0;
	  fgets(str,300,file);
	  sscanf(str,"%d " ,&polygon_size);

	  cout << "polygon "<<polygon_size<<endl;

	  do
	  {
		  cout << point <<endl;
		  Face temp;
		  int pos[6];
		  if(polygon_size == 3)
		  {

			  sscanf(str,"%d %d %d %d ",&polygon_size,&pos[0],&pos[1],&pos[2]);

			  printf("faces %d %d %d %d \n",polygon_size,pos[0],pos[1],pos[2]);
			  temp.addVertexIndex(pos[0]);
			  temp.addVertexIndex(pos[1]);
			  temp.addVertexIndex(pos[2]);
			  
		  }
		  if(polygon_size == 4)
		  {

			  sscanf(str,"%d %d %d %d %d",&polygon_size,&pos[0],&pos[1],&pos[2],&pos[3]);

			  printf("faces %d %d %d %d %d\n",polygon_size,pos[0],pos[1],pos[2],pos[3]);
			  temp.addVertexIndex(pos[0]);
			  temp.addVertexIndex(pos[1]);
			  temp.addVertexIndex(pos[2]);
			  temp.addVertexIndex(pos[3]);
		  }
		  if(polygon_size == 5)
		  {
			  sscanf(str,"%d %d %d %d %d %d",&polygon_size,&pos[0],&pos[1],&pos[2],&pos[3],&pos[4]);

			  printf("faces %d %d %d %d %d",pos[0],pos[1],pos[2],pos[3],pos[4]);
			  temp.addVertexIndex(pos[0]);
			  temp.addVertexIndex(pos[1]);
			  temp.addVertexIndex(pos[2]);
			  temp.addVertexIndex(pos[3]);
			  temp.addVertexIndex(pos[4]);
		  }
		  faces.push_back(temp);
		  fgets(str,300,file);
		  point++;
	  }while(point<num_face);
	  fclose(file);
	cout << "faces"<<faces.size();
}

void Mesh::triangulation()
{	vector<Face> temp;
	if(polygon_size == 4)
	{
		for(int i=0;i<num_face;i++)
		{
			vector<int> use;
			use = faces[i].getVertexIndices();
			Face f1,f2;

			f1.addVertexIndex(use[0]);
			f1.addVertexIndex(use[1]);
			f1.addVertexIndex(use[2]);

			f2.addVertexIndex(use[0]);
			f2.addVertexIndex(use[2]);
			f2.addVertexIndex(use[3]);

			temp.push_back(f1);
			temp.push_back(f2);

		}
	}
	if(polygon_size == 5)
	{
		for(int i=0;i<faces.size();i++)
				{
			vector<int> use;
						use = faces[i].getVertexIndices();
						Face f1,f2,f3;

						f1.addVertexIndex(use[0]);
						f1.addVertexIndex(use[1]);
						f1.addVertexIndex(use[2]);

						f2.addVertexIndex(use[0]);
						f2.addVertexIndex(use[2]);
						f2.addVertexIndex(use[3]);

						f3.addVertexIndex(use[0]);
						f3.addVertexIndex(use[3]);
						f3.addVertexIndex(use[4]);
						temp.push_back(f1);
						temp.push_back(f2);
						temp.push_back(f3);


				}
	}
	if(polygon_size!=3){
	faces.clear();
	faces = temp;
	num_face = temp.size();
	temp.clear();
	cout << "\n faces after triangulation" <<faces . size();
	polygon_size=3;
	}
	
}
void Mesh::modify(vector<Vertex> meshVertices,vector<Face> meshFaces,int mesh_polygon_size)
{
polygon_size = mesh_polygon_size;
edges.clear();
neighbours.clear();
vertices.clear();
vertices = meshVertices;
num_ver = vertices.size();
faces.clear();
faces = meshFaces;
num_face = faces.size();
triangulation();
make_edges();
}
void Mesh::make_edges()
{	int edge_count=0;
	cout << "Inside make_edges";
	for(int i=0;i<num_face;i++)
	{
		vector<int> fv;
		fv = faces[i].getVertexIndices();
		Edge e1,e2,e3;
		//cout << fv[0] <<" "<<fv[1]<< " "<<fv[2]<<"\n";

		e1.set_vertex(vertices[fv[0]],vertices[fv[1]]);
		int pos = find_edge(e1);
		//cout << " edge 1 : "<<pos<<endl;
		if(pos!=(-1))
		{
			faces[i].addEdgeIndex(pos);
			edges[pos].add_opposite(vertices[fv[2]]);
		}
		else
			{
			e1.add_opposite(vertices[fv[2]]);
			edges.push_back(e1);
			faces[i].addEdgeIndex(edge_count++);
			}
		//cout << "edge :"<<fv[0]<<";"<<fv[1]<<"oppo:"<<fv[2];

		e2.set_vertex(vertices[fv[1]],vertices[fv[2]]);
		pos = find_edge(e2);
		//cout << " edge 2 : "<<pos<<endl;
		if(pos!=(-1))
		{
			faces[i].addEdgeIndex(pos);
			edges[pos].add_opposite(vertices[fv[0]]);
		}
		else
		{
			e2.add_opposite(vertices[fv[0]]);
			edges.push_back(e2);
			faces[i].addEdgeIndex(edge_count++);
		}
		//cout << "edge :"<<fv[1]<<";"<<fv[2]<<"oppo:"<<fv[0];

		e3.set_vertex(vertices[fv[2]],vertices[fv[0]]);
		pos = find_edge(e3);
		//cout << " edge 3 : "<<pos<<endl;
		if(pos!=(-1)){
			edges[pos].add_opposite(vertices[fv[1]]);
			faces[i].addEdgeIndex(pos);
		}

		else
			{
			e3.add_opposite(vertices[fv[1]]);
			edges.push_back(e3);
			faces[i].addEdgeIndex(edge_count++);
			}
		//cout << "edge :"<<fv[0]<<";"<<fv[2]<<"oppo:"<<fv[1];

	}
	num_edges = edges.size();
	make_neighbours();
	 cout << "Edges Complete" << edges.size()<<endl;
	 for (int i=0;i<edges.size();i++)
	 {
	 	vector<Vertex> e = edges[i].get_vertex();
	 	vector<Vertex> v = edges[i].get_opposites();
	 	cout <<"Edges "<<i<<": vertex"<<endl;
	 		cout<<e[0].get_x()<<" " << e[0].get_y()<<" "<<e[0].get_z()<<endl;
	 		cout<<e[1].get_x()<<" " << e[1].get_y()<<" "<<e[1].get_z()<<endl;
	 		cout<<"opposites "<<endl;
	 	for(int j=0;j<v.size();j++)
	 	{
	 		cout<<v[j].get_x()<<" " << v[j].get_y()<<" "<<v[j].get_z()<<endl;
	 	}
	}
}
int Mesh::find_edge(Edge e)
		{
	    int pos1=-1;
		for(int i=0;i<edges.size();i++)
		{
			vector<Vertex> v;
			vector<Vertex> ve;
			ve = e.get_vertex();
			v= edges[i].get_vertex();
			if((find(v.begin(),v.end(),ve[0])!=v.end())&&(find(v.begin(),v.end(),ve[1])!=v.end()))
			{
				pos1=i;
			}
		}
		return pos1;
		}

void Mesh::make_neighbours()
{	neighbours.clear();
	for(int i=0;i<num_ver;i++)
	{
		vector<Vertex> nghbr;
		cout<<"\n"<<num_edges<<endl;
		for(int j=0;j<edges.size();j++)
		{
			vector<Vertex> ev;
			ev=edges[j].get_vertex();
			// cout << "Edge : vertex  " <<j << " " << i<<endl;;
			// cout << ev[0].get_x()<<" "<<ev[0].get_y()<<" "<<ev[0].get_z()<<endl;;
			// cout << ev[1].get_x()<<" "<<ev[1].get_y()<<" "<<ev[1].get_z()<<endl;;
			if(ev[0]==vertices[i])
				nghbr.push_back(ev[1]);
			if(ev[1]==vertices[i])
				nghbr.push_back(ev[0]);
		}
		//cout << "next" << endl;
		neighbours.push_back(nghbr);
	}
	for(int i=0;i<num_ver;i++){cout<< "Vertex : "<<i; vector<Vertex> v = get_neighbours(i);
		for(int j=0;j<v.size();j++)
			cout << v[j].get_x()<<" "<<v[j].get_y()<<" "<<v[j].get_z()<<endl;;
	}
}
int Mesh::find_vertex(vector<Vertex> ee, vector<Vertex> ef)
{
	int pos;
	for(int i=0;i<3;i++)
			if(find(ee.begin(),ee.end(),ef[i])==ee.end())
				pos= i;
	return pos;
}
vector<int> Mesh::find_ad_vertex(int index,int  face)
{

	vector <int> target;
	vector<int> edi = faces[face].getEdgeIndices();
	// search for the edges in the faces which has the incoming vertex;
	for(int i=0;i< 3; i++)
	{
		vector<Vertex> vi = edges[edi[i]].get_vertex();
		if((vi[0]==vertices[index])||(vi[1]==vertices[index]))
			target.push_back(edi[i]);
		
	}
	
	return target;
}
void Mesh::subdivide()
{
	cout << "Inside subdivide "<<endl;
	vector<Vertex> vexisting;
	vector<Vertex> ngh;
	for(int i=0;i<num_ver;i++)
	{
		Vertex vn;
		float x,y,z;
		float wt1=0,wt2=0;
		ngh = get_neighbours(i);
		int nsize ;
		nsize = ngh.size();
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

		
		x = (vertices[i].get_x()*(wt1));
		for(int j=0; j< ngh.size();j++ )
			{
			x = x + (ngh[j].get_x()*wt2);
			cout <<"x : " <<x<<endl;;
			}
		y = (vertices[i].get_y()*wt1);
		for(int j=0; j< ngh.size();j++ )
			{
				y = y + (ngh[j].get_y()*wt2);
			}
		z = (vertices[i].get_z()*wt1);
		for(int j=0; j< ngh.size();j++ )
			{
				z = z + (ngh[j].get_z()*wt2);
			}
			vn.set_coords(x,y,z);
		vexisting.push_back(vn);
	}
	
	 vector<Vertex> vnew;
	 vector<Vertex> edge_vertices;
	 vector<Vertex> edge_opposites;
	//making the new vertices;
	for(int i=0;i<num_edges;i++)
	{
		
		edge_vertices = edges[i].get_vertex();
		edge_opposites = edges[i].get_opposites();
		Vertex v ;
		float x,y,z;
		x = (edge_vertices[0].get_x()*0.375)+(edge_vertices[1].get_x()*0.375)
			 + (edge_opposites[0].get_x()*0.125) + (edge_opposites[1].get_x() * 0.125);
		y = (edge_vertices[0].get_y()*0.375)+(edge_vertices[1].get_y()*0.375) 
			 + (edge_opposites[0].get_y()*0.125) + (edge_opposites[1].get_y() * 0.125);
		z = (edge_vertices[0].get_z()*0.375)+(edge_vertices[1].get_z()*0.375)
			 + (edge_opposites[0].get_z()*0.125) + (edge_opposites[1].get_z() * 0.125);
		v.set_coords(x,y,z);
		vnew.push_back(v);
	}
	
	//retriangulation
	vector<Face> faceNew;
	vector<int> edi;
	cout<< "num_face"<< num_face;
 	for(int i=0;i<num_face;i++)
	{
		//let the vertex be comprised of the existing modified vertices first then the 
		//new calculated vertices.
		cout << "face " << i<<endl;
		vector<int> vi  = faces[i].getVertexIndices();
		vector<int>	ei = faces[i].getEdgeIndices();

		//forming the face with the new vertices firsvertices.clear();
	
		//The new vertices are found in the vnew, since the index would be the same of the edges of the faces
		//the edge indices would be used.


		Face f1,f2,f3,f4;
		f1.addVertexIndex(vexisting.size()+ei[0]);//while forming the new vertex array the existing modified vertices 
		f1.addVertexIndex(vexisting.size()+ei[1]);//would get the first preference.
		f1.addVertexIndex(vexisting.size()+ei[2]);

		//each of the new faces would be formed  by the combination of the 1 existing vertex and two new vertex

		//the new vertices to be used can be found by checking the edges which has the existing edge and then use 
		//that  edges index;

		f2.addVertexIndex(vi[0]);
		edi = find_ad_vertex(vi[0],i);
		f2.addVertexIndex(vexisting.size()+ edi[0]);
		f2.addVertexIndex(vexisting.size()+ edi[1]);
		edi.clear();


		f3.addVertexIndex(vi[1]);
		edi = find_ad_vertex(vi[1],i);
		f3.addVertexIndex(vexisting.size()+ edi[0]);
		f3.addVertexIndex(vexisting.size()+ edi[1]);
		edi.clear();

		f4.addVertexIndex(vi[2]);
		edi = find_ad_vertex(vi[2],i);
		f4.addVertexIndex(vexisting.size()+ edi[0]);
		f4.addVertexIndex(vexisting.size()+ edi[1]);
		edi.clear();

		// The new faces gernerated are then inserted in the faceNew vector.

		faceNew.push_back(f1);
		faceNew.push_back(f2);
		faceNew.push_back(f3);
		faceNew.push_back(f4);
		
	
	}
	//After the new faces are formed we would actually have to replace the vertex array with
	//the modified vertices and then the new vertices
	vertices.clear();
	vertices = vexisting;
	vertices.insert(vertices.end(),vnew.begin(),vnew.end());
	num_ver=vertices.size();
	// face array would have to be flushed and te faceNew array now becomes the new face array;
	faces.clear();
	faces = faceNew;
	num_face = faces.size();
	//the edge array would have to cleared and the made again()
	edges.clear();
	make_edges();



	for(int i=0;i<faces.size();i++)
	{
		vector<int> vi = faces[i].getVertexIndices();
		vector<int> ei = faces[i].getEdgeIndices();
		cout << "face : "<< i <<endl;
		for (int j=0;j<vi.size();j++)
		{
			cout << " Vertex " << vertices[vi[j]].get_x()<<" " <<vertices[vi[j]].get_y()<<" " << vertices[vi[j]].get_z() <<endl;

		}
		
	}
}





void Mesh::normal(Vertex p1, Vertex p2, Vertex p3)
	{
		float nx, ny, nz;
		nx = (p1.get_y())*((p2.get_z())-(p3.get_z()))+(p2.get_y())*((p3.get_z())-(p1.get_z()))+(p3.get_y())*((p1.get_z())-(p2.get_z()));
		ny = (p1.get_z())*((p2.get_x())-(p3.get_x()))+(p2.get_z())*((p3.get_x())-(p1.get_x()))+(p3.get_z())*((p1.get_x())-(p2.get_x()));
		nz = (p1.get_x())*((p2.get_y())-(p3.get_y()))+(p2.get_x())*((p3.get_y())-(p1.get_y()))+(p3.get_x())*((p1.get_y())-(p2.get_y()));
		glNormal3f(nx, ny, nz);
	}
 void Mesh::draw(int controlmesh)
 {
	if(controlmesh == 1)
	{
	for(int i=0;i<num_face;i++)
	    {
		vector<int> indices = faces[i].getVertexIndices();
		glBegin(GL_LINE_LOOP);
		 for(int j=0;j<polygon_size;j++)
		 {
		 glVertex3f(vertices[indices[j]].get_x(),vertices[indices[j]].get_y(),vertices[indices[j]].get_z());
		 }
		glEnd();
	    }
	}
 	else{
	 for(int i=0;i<num_face;i++)
	 {
		 vector<int> indices = faces[i].getVertexIndices();
		 glBegin(GL_TRIANGLES);
		 normal(vertices[indices[0]],vertices[indices[1]],vertices[indices[2]]);
		 for(int j=0;j<3;j++)
		 {

			 
			 glVertex3f(vertices[indices[j]].get_x(),vertices[indices[j]].get_y(),vertices[indices[j]].get_z());
		 }glEnd();

	 }
	 }

}



































































	// int face_count;
	// vector<Face> fnew;
	// for(int i=0;i<num_face;i++)
	// {
	// 	vector<int> edi = faces[i].getEdgeIndices();
	// 	vector<int> vdi = faces[i].getVertexIndices();
	// 	cout<<"parent vertex 1::"<<vdi[0]<<"  vertex 2::"<<vdi[1]<<" vertex 3::"<<vdi[2];
	// 	Face f1,f2,f3,f4;

	// 	f1.addVertexIndex(edi[0]+vexisting.size());
	// 	cout<< " vertex 1 ::"<<edi[0]+vexisting.size();
	// 	f1.addVertexIndex(edi[1]+vexisting.size());
	// 	cout<< " vertex 2 ::"<<edi[1]+vexisting.size();
	// 	f1.addVertexIndex(edi[2]+vexisting.size());
	// 	cout<< " vertex 3 ::"<<edi[2]+vexisting.size()<<endl;

	// 	fnew.push_back(f1);

	// 	f2.addVertexIndex(vdi[0]);
	// 	vector<int> adver = find_ad_vertex(vdi[0],i);
	// 	cout << " vertex 1 ::"<<vdi[0];
	// 	f2.addVertexIndex(adver[0]+vexisting.size());
	// 	cout<< " vertex 2 ::"<<adver[0]+vexisting.size();
	// 	f2.addVertexIndex(adver[1]+vexisting.size());
	// 	cout<< " vertex 3 ::"<<adver[1]+vexisting.size()<<endl;
	// 	fnew.push_back(f2);

	// 	adver.clear();
	// 	f3.addVertexIndex(vdi[1]);
	// 	cout << " vertex 1 ::"<<vdi[1];
	// 	adver = find_ad_vertex(vdi[1],i);
	// 	f3.addVertexIndex(adver[0]+vexisting.size());
	// 	cout<< " vertex 2 ::"<<adver[0]+vexisting.size();
	// 	f3.addVertexIndex(adver[1]+vexisting.size());
	// 	cout<< " vertex 3 ::"<<adver[1]+vexisting.size()<<endl;
	// 	fnew.push_back(f3);

	// 	adver.clear();
	// 	f4.addVertexIndex(vdi[2]);
	// 	cout << " vertex 1 ::"<<vdi[2];
	// 	adver = find_ad_vertex(vdi[2],i);
	// 	cout<< " vertex 2 ::"<<adver[0]+vexisting.size();
	// 	f4.addVertexIndex(adver[0]+vexisting.size());
	// 	f4.addVertexIndex(adver[1]+vexisting.size());
	// 	cout<< " vertex 3 ::"<<adver[1]+vexisting.size()<<endl;
	// 	fnew.push_back(f4);



	// }
	// faces.clear();
	// faces = fnew;
	// num_edges=edges.size();
	// vertices.clear();
	// vertices = vexisting;
	// vertices.insert(vertices.end(),vnew.begin(),vnew.end());
	// num_ver=vertices.size();
	// edges.clear();

	// make_edges();

	// num_face=faces.size();
	// cout << faces.size() <<"  "<< edges.size(); 
