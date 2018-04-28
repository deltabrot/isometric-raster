#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "vertex.h"

class Triangle{
	public:
		Vertex vert[3], project, rotationPoint;
		float xCoords[3], yCoords[3];
		float rotation;
		Triangle(Vertex v0, Vertex v1, Vertex v2, Vertex project, float rotation, Vertex rotationPoint);
		void get2dCoords();

};

#endif /* __TRIANGLE_H__ */
