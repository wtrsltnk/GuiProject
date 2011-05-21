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
#define WORLD 8192.0

class Brush;

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
	void updateFromVertices(Brush* brush);

	static bool getIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& out);
	static Plane fromVertices(const Vector3& v1, const Vector3& v2, const Vector3& v3);
};

class Brush
{
public:
	Brush();
	virtual ~Brush();

	// Adds a plane to this brush
	void addPlane(Plane& plane);

	// All the planes a brush is made of
	std::vector<Plane> mPlanes;

	// All the vertices of this brush
	std::vector<Vector3> mVertices;

	// Bounding of this brush
	float mMins[3], mMaxs[3];

	// The color of this plane
	float mColor[4];

	// Gets the index of a  vertex at the given position
	int getIndexOf(const Vector3& vertex);

	// Updates all the vertices of this brush
	void updateVertices();

	// Updates the bounding box of this brush
	void updateBounds();
	
	// Move the brush
	void move(float x, float y, float z);
	
	// Scale the brush
	void scale(float x, float y, float z, const Vector3& origin);
	
	// Rotate the brush
	void rotate(float x, float y, float z, const Vector3& origin);

	// Determine if the given position is within this world
	static bool pointInWorld(const Vector3& point)
	{
		if(point.x() < -WORLD || point.x() > WORLD)
			return false;

		if(point.y() < -WORLD || point.y() > WORLD)
			return false;

		if(point.z() < -WORLD || point.z() > WORLD)
			return false;

		return true;
	}
};

}

#endif	/* BRUSH_H */

