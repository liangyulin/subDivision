#pragma once
#include <vector>

using namespace std;

//IO结构
typedef struct wrlVertexStruct{
	float x;
	float y;
	float z;
}Vertexs;

typedef struct wrlFaceStruct{
	int vertex0;
	int	vertex1;
	int	vertex2;
}Faces;


//half-edge结构
typedef struct vertexStruct{
	float x;
	float y;
	float z;
	int incidentEdge;
}HEVertex;

typedef struct faceStruct{
	int edge;
}HEFace;

typedef struct half_EdgeStruct{
	int origin;
	int twin;
	int incidentFace;
	int next;
	int prev;
}HEHalfEdge;