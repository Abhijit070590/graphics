#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
	Face*& getFace() {
		return face;
	}

	void setFace(Face*& face) {
		this->face = face;
	}

	Edge*& getNext() {
		return next;
	}

	void setNext(Edge*& next) {
		this->next = next;
	}

	Edge*& getPair() {
		return pair;
	}

	void setPair(Edge*& pair) {
		this->pair = pair;
	}

	Edge*& getPrev() {
		return prev;
	}

	void setPrev(Edge*& prev) {
		this->prev = prev;
	}

	Vertex*& getVert() {
		return vert;
	}

	void setVert(Vertex*& vert) {
		this->vert = vert;
	}

private:
  	Vertex* vert;
	Face* face;
  	Edge* next;
  	Edge* prev;
  	Edge* pair;
};

#endif