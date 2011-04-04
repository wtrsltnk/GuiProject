/* 
 * File:   Brush.h
 * Author: wouter
 *
 * Created on April 1, 2011, 10:56 PM
 */

#ifndef BRUSH_H
#define	BRUSH_H

#include <vector>

namespace geo
{

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z);
	virtual ~Vertex();

	// A trick to be able to use an instance of this class as parameter for glVertex3fv();
	operator const float* () const { return this->mXyz; }

	//The position  of this vertex
	float mXyz[3];

};

class Plane
{
public:
	Plane();
	Plane(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	virtual ~Plane();

	// The three vertices defining the base of this plane
	Vertex mBase[3];

	// Indices into the Brush vertex array. These vertices describe the face for this plane
	std::vector<int> mIndices;

	// The color of this plane
	float mColor[4];

};

class Brush
{
public:
	Brush();
	virtual ~Brush();
	
	void addPlane(Plane& plane);

	// All the planes a brush is made of
	std::vector<Plane> mPlanes;

	// The current selected plane from this Brush
	Plane* mSelectedPlane;

	// All the vertices of this brush
	std::vector<Vertex> mVertices;

	// Updates all the vertices of this brush
	void updateVertices();

};

}

#endif	/* BRUSH_H */

