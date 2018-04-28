#include <iostream>
#include <cmath>
#include "triangle.h"
#include "rasterization.h"
#include "color.h"
#include "vertex.h"

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2, Vertex project, float rotation, Vertex rotationPoint){
	this->vert[0] = v0;	
	this->vert[1] = v1;	
	this->vert[2] = v2;	
	this->project = project;
	this->rotation = rotation;
	this->rotationPoint = rotationPoint;
}

void Triangle::get2dCoords(){
	float cpio6 = cosf(M_PI/6.0f);
	float spio6 = sinf(M_PI/6.0f);
	float xCoords[] = {((vert[0].x + project.x)*cpio6) + ((vert[0].y + project.y)*cpio6),
			   ((vert[1].x + project.x)*cpio6) + ((vert[1].y + project.y)*cpio6),
			   ((vert[2].x + project.x)*cpio6) + ((vert[2].y + project.y)*cpio6)};

	float yCoords[] = {(vert[0].x + project.x)*spio6 - (vert[0].y + project.y)*spio6 - (vert[0].z + project.z),
			   (vert[1].x + project.x)*spio6 - (vert[1].y + project.y)*spio6 - (vert[1].z + project.z),
			   (vert[2].x + project.x)*spio6 - (vert[2].y + project.y)*spio6 - (vert[2].z + project.z)};
	
	float xCoordsDouble[3];
	float yCoordsDouble[3];
	//std::cout<<vert[1].x+project.x<< ", "<<std::endl;

	//Find the 2d coords of the rotation point
        float rotationPointX = rotationPoint.x*cpio6 + rotationPoint.y*cpio6;
        float rotationPointY = rotationPoint.x*spio6 - rotationPoint.y*spio6 + rotationPoint.z;
	
	//Loop through all vertices
	for(int i=0;i<3;i++){
		//rotateAngle is the current angle from the rotation point's 2d vertex coords to the triangle's 2d coords
		float rotateAngle = M_PI/2;
		if((yCoords[i]+vert[i].z)-rotationPointY != 0){
			rotateAngle = atan((xCoords[i]-rotationPointX)/(sqrt(3.0f)*((yCoords[i]+vert[i].z)-rotationPointY)));
		}
		float rotationConstX, rotationConstY;
		
		if(std::abs(rotateAngle-M_PI) >= 0.01 ){
			rotationConstX = (xCoords[i] - rotationPointX)/sinf(rotateAngle);
		}
		else{
			rotationConstX = ((yCoords[i]+vert[i].z)-rotationPointY)*sqrt(3);
		}

		if(std::abs(rotateAngle-(M_PI/2)) >= 0.01){
			rotationConstY = ((yCoords[i]+vert[i].z)-rotationPointY)/cosf(rotateAngle);
		}
		else{
			rotationConstY = (xCoords[i]-rotationPointX)/sqrt(3);
		}
		//std::cout<<rotateAngle<<std::endl;
		xCoordsDouble[i] = ((rotationPointX) + (rotationConstX * cosf(rotateAngle+rotation-(M_PI/2.0f))));
		yCoordsDouble[i] = ((rotationPointY) - (rotationConstY * sinf(rotateAngle+rotation-(M_PI/2.0f))) - vert[i].z);
	}
	this->xCoords[0] = xCoordsDouble[0];
	this->xCoords[1] = xCoordsDouble[1];
	this->xCoords[2] = xCoordsDouble[2];
	this->yCoords[0] = yCoordsDouble[0];
	this->yCoords[1] = yCoordsDouble[1];
	this->yCoords[2] = yCoordsDouble[2];

}
