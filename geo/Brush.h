/* 
 * File:   Brush.h
 * Author: wouter
 *
 * Created on April 1, 2011, 10:56 PM
 */

#ifndef BRUSH_H
#define	BRUSH_H

#include <vector>
#include "../common/vector3.h"

namespace geo
{

#define EPSILON 0.01

class Plane
{
public:
	Plane();
	Plane(const Vector3& normal, float distance);
	virtual ~Plane();

	Vector3 mNormal;
	float mDistance;

	// Indices into the Brush vertex array. These vertices describe the face for this plane
	std::vector<int> mIndices;
	Vector3 average;
	
	int getIndexOf(int vertexIndex);

	static bool getIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& out);
	static Plane fromVertices(const Vector3& v1, const Vector3& v2, const Vector3& v3);
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
	std::vector<Vector3> mVertices;

	// The color of this plane
	float mColor[4];
	
	int getIndexOf(const Vector3& vertex);

	// Updates all the vertices of this brush
	void updateVertices();
	Vector3 calculateOrigin();

};

}

#endif	/* BRUSH_H */

