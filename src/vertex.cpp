#include <iostream>
#include <cmath>
#include "vertex.h"

Vertex::Vertex(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Vertex::Vertex(){
	x = y = z = 0;
}

void Vertex::print(){
	std::cout << "x: " << x << "\ty: " << y << "\tz: " << z << std::endl;
}

Vertex Vertex::operator + (const Vertex &v) const{
	return Vertex(x + v.x, y + v.y, z + v.z);
}

Vertex Vertex::operator - (const Vertex &v) const{
	return Vertex(x - v.x, y - v.y, z - v.z);
}

float Vertex::operator * (const Vertex &v) const{
	return ((x * v.x) + (y * v.y) + (z * v.z));
}
