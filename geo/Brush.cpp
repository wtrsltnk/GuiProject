/* 
 * File:   Brush.cpp
 * Author: wouter
 * 
 * Created on April 1, 2011, 10:56 PM
 */

#include "Brush.h"

namespace geo
{

Vertex::Vertex()
{
	this->mXyz[0] = 0;
	this->mXyz[1] = 0;
	this->mXyz[2] = 0;
}

Vertex::Vertex(float x, float y, float z)
{
	this->mXyz[0] = x;
	this->mXyz[1] = y;
	this->mXyz[2] = z;
}

Vertex::~Vertex()
{
}

static int planeCount = 0;

Plane::Plane()
{
	this->mColor[0] = 0;
	this->mColor[1] = 0;
	this->mColor[2] = (155 + (planeCount*10) % 99) / 255.0f;
	this->mColor[3] = 1.0f;
}

Plane::Plane(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	this->mBase[0] = v1;
	this->mBase[1] = v2;
	this->mBase[2] = v3;
}

Plane::~Plane()
{
}

Brush::Brush()
{
}

Brush::~Brush()
{
}

void Brush::addPlane(Plane& plane)
{
	this->mPlanes.push_back(plane);
}

void Brush::updateVertices()
{
}

}
