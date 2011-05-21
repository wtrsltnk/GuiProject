/* 
 * File:   Brush.cpp
 * Author: wouter
 * 
 * Created on April 1, 2011, 10:56 PM
 */

#include "Brush.h"
#include "../common/matrix4x4.h"
#include <cmath>
#include <map>
#include <stdio.h>
#include <stdlib.h>

namespace geo
{


/******************************************************************/
/* Plane														  */
/******************************************************************/
Plane::Plane()
{
}

Plane::Plane(const Vector3& normal, float distance)
	: mNormal(normal), mDistance(distance)
{
}

Plane::~Plane()
{
}

int Plane::getIndexOf(int vertexIndex)
{
	for (int i = 0; i < this->mIndices.size(); i++)
		if (this->mIndices[i] == vertexIndex)
			return i;
	return -1;
}

void Plane::updateFromVertices(Brush* brush)
{
	Vector3 v1(brush->mVertices[this->mIndices[0]]);
	Vector3 v2(brush->mVertices[this->mIndices[1]]);
	Vector3 v3(brush->mVertices[this->mIndices[2]]);
	
	this->mNormal = (v3-v2).crossProduct(v1-v2).unit();
	this->mDistance = this->mNormal.dotProduct(v2);
}

// This is magic from Nemesis MapViewer. ToDo: create my own code for this
bool Plane::getIntersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3& out)
{
	//float fDenom = p1.mNormal->Dot(p2.mNormal->Cross(p3.mNormal));

	//if(fDenom == 0.0f)
		//return false;

	float fDet;
	float MN[9] = { p1.mNormal.x(), p1.mNormal.y(), p1.mNormal.z(), p2.mNormal.x(), p2.mNormal.y(), p2.mNormal.z(), p3.mNormal.x(), p3.mNormal.y(), p3.mNormal.z() };
	float IMN[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	float MD[3] = { p1.mDistance, p2.mDistance , p3.mDistance };

	//fDet = MN[0] * (MN[4] * MN[8] - MN[5] * MN[7]);
	//fDet += -MN[1] * (MN[3] * MN[8] - MN[5] * MN[6]);
	//fDet += MN[2] * (MN[3] * MN[7] - MN[4] * MN[6]);

	/*fDet = MN[0] * MN[4] * MN[8]
		 + MN[1] * MN[5] * MN[6]
		 + MN[2] * MN[3] * MN[7]
		 - MN[2] * MN[4] * MN[6]
		 - MN[0] * MN[5] * MN[7]
		 - MN[1] * MN[3] * MN[8];*/

	IMN[0] = MN[4] * MN[8] - MN[5] * MN[7];
	IMN[3] = -(MN[3] * MN[8] - MN[5] * MN[6]);
	IMN[6] = MN[3] * MN[7] - MN[4] * MN[6];

	fDet = MN[0] * IMN[0] + MN[1] * IMN[3] + MN[2] *IMN[6];

	if(fDet == 0.0f)
		return false;

	IMN[1] = -(MN[1] * MN[8] - MN[2] * MN[7]);
	IMN[4] = MN[0] * MN[8] - MN[2] * MN[6];
	IMN[7] = -(MN[0] * MN[7] - MN[1] * MN[6]);
	IMN[2] = MN[1] * MN[5] - MN[2] * MN[4];
	IMN[5] = -(MN[0] * MN[5] - MN[2] * MN[3]);
	IMN[8] = MN[0] * MN[4] - MN[1] * MN[3];

	fDet = 1.0f / fDet;

	IMN[0] *= fDet;
	IMN[1] *= fDet;
	IMN[2] *= fDet;
	IMN[3] *= fDet;
	IMN[4] *= fDet;
	IMN[5] *= fDet;
	IMN[6] *= fDet;
	IMN[7] *= fDet;
	IMN[8] *= fDet;

	out.x(IMN[0] * MD[0] + IMN[1] * MD[1] + IMN[2] * MD[2]);
	out.y(IMN[3] * MD[0] + IMN[4] * MD[1] + IMN[5] * MD[2]);
	out.z(IMN[6] * MD[0] + IMN[7] * MD[1] + IMN[8] * MD[2]);

	return true;
}

Plane Plane::fromVertices(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = (v3-v2).crossProduct(v1-v2).unit();
	float distance = normal.dotProduct(v2);

	return Plane(normal, distance);
}


/******************************************************************/
/* Brush														  */
/******************************************************************/
static int brushCount = 0;

Brush::Brush()
{
	this->mColor[0] = float(rand() % 255) / 255.0f;
	this->mColor[1] = float(rand() % 255) / 255.0f;
	this->mColor[2] = float(rand() % 255) / 255.0f;
//	this->mColor[brushCount%3] = (155 + (brushCount*10) % 99) / 255.0f;
//	this->mColor[(brushCount+1)%3] = (155 + (brushCount*7) % 99) / 255.0f;
	this->mColor[3] = 1.0f;

	brushCount++;
}

Brush::~Brush()
{
}

void Brush::addPlane(Plane& plane)
{
	this->mPlanes.push_back(plane);
}

int Brush::getIndexOf(const Vector3& vertex)
{
	for (int i = 0; i < this->mVertices.size(); i++)
		if (this->mVertices[i] == vertex)
			return i;
	return -1;
}

float calculateSignedAngle(const Vector3& v1, const Vector3& v2, const Vector3& normal)
{
	Vector3 c = v2.crossProduct(v1);
	float angle = std::atan2(double((v2-v1).length()), double(v2.dotProduct(v1)));
	return c.dotProduct(normal) < float(0) ? -angle : angle;
}

void Brush::updateVertices()
{
	// Reset the boundingbox
	this->mMins[0] = this->mMins[1] = this->mMins[2] =  99999.9f;
	this->mMaxs[0] = this->mMaxs[1] = this->mMaxs[2] = -99999.9f;
	
	// Loop through all the planes, and gather 3 different planes to create a vertex from
	for(int i = 0; i < this->mPlanes.size() - 2; i++)
	{
		for(int j = 0; j < this->mPlanes.size() - 1; j++)
		{
			for(int k = 0; k < this->mPlanes.size(); k++)
			{
				// Only different planes!
				if(i != j && i != k && j != k)
				{
					Vector3 intersection;
					// Determine the intersection point of these three Planes
					if (Plane::getIntersection(this->mPlanes[i], this->mPlanes[j], this->mPlanes[k], intersection))
					{
						// Make sure the intersection lies in this world
						if (Brush::pointInWorld(intersection))
						{
							bool bLegal = true;

							// Check every other plane if the intersection is legal.
							for(int l = 0; l < this->mPlanes.size(); l++)
							{
								if(l != i && l != j && l != k)
								{
									// Only when the intersection is on the correct side of all other planes
									float dist = this->mPlanes[l].mNormal.dotProduct(intersection) - this->mPlanes[l].mDistance;
									if(dist < EPSILON)
									{
										bLegal = false;
										break;
									}
								}
							}

							if (bLegal)
							{
								// Check if we need to add the point, of can index an existing point from the brush
								int index = this->getIndexOf(intersection);
								if (index == -1)
								{
									index = this->mVertices.size();
									this->mVertices.push_back(intersection);

									// Update the bounding box
									if (this->mMins[0] > intersection.x()) this->mMins[0] = intersection.x();
									if (this->mMins[1] > intersection.y()) this->mMins[1] = intersection.y();
									if (this->mMins[2] > intersection.z()) this->mMins[2] = intersection.z();

									if (this->mMaxs[0] < intersection.x()) this->mMaxs[0] = intersection.x();
									if (this->mMaxs[1] < intersection.y()) this->mMaxs[1] = intersection.y();
									if (this->mMaxs[2] < intersection.z()) this->mMaxs[2] = intersection.z();
								}

								// Only add the point when it is not yet in the index list
								if (this->mPlanes[i].getIndexOf(index) == -1) this->mPlanes[i].mIndices.push_back(index);
								if (this->mPlanes[j].getIndexOf(index) == -1) this->mPlanes[j].mIndices.push_back(index);
								if (this->mPlanes[k].getIndexOf(index) == -1) this->mPlanes[k].mIndices.push_back(index);
							}
						}
					}
				}
			}
		}
	}

	// Now we need to make sure all vertices of the planes are ordered CCW
	for(int i = 0; i < this->mPlanes.size(); i++)
	{
		std::map<float, int> indices;

		// Determine an avarage over all points in this face
		for(int j = 0; j < this->mPlanes[i].mIndices.size(); j++)
			this->mPlanes[i].average += this->mVertices[this->mPlanes[i].mIndices[j]];
		this->mPlanes[i].average *= (1.0f / this->mPlanes[i].mIndices.size());

		// Do we have at least one point?
		if (this->mPlanes[i].mIndices.size() > 0)
		{
			Vector3 start = this->mVertices[this->mPlanes[i].mIndices[0]];
			indices.insert(std::make_pair(0, this->mPlanes[i].mIndices[0]));
			for (int j = 1; j < this->mPlanes[i].mIndices.size(); j++)
			{
				// Calculate the angle around our average and save it in a map. This map will sort these angles
				float angle = calculateSignedAngle(start-this->mPlanes[i].average, this->mVertices[this->mPlanes[i].mIndices[j]]-this->mPlanes[i].average, this->mPlanes[i].mNormal);
				indices.insert(std::make_pair(angle, this->mPlanes[i].mIndices[j]));
			}

			// Clear the original indices array
			this->mPlanes[i].mIndices.clear();

			// Add the correct order of the indices from the map into the indices array
			for (std::map<float, int>::iterator itr = indices.begin(); itr != indices.end(); ++itr)
				this->mPlanes[i].mIndices.push_back(itr->second);
		}
	}
}

void Brush::updateBounds()
{
	// Reset the boundingbox
	this->mMins[0] = this->mMins[1] = this->mMins[2] =  99999.9f;
	this->mMaxs[0] = this->mMaxs[1] = this->mMaxs[2] = -99999.9f;
	
	for (std::vector<Vector3>::iterator itr = this->mVertices.begin(); itr != this->mVertices.end(); ++itr)
	{
		if ((*itr).x() > this->mMaxs[0]) this->mMaxs[0] = (*itr).x();
		if ((*itr).y() > this->mMaxs[1]) this->mMaxs[1] = (*itr).y();
		if ((*itr).z() > this->mMaxs[2]) this->mMaxs[2] = (*itr).z();
		
		if ((*itr).x() < this->mMins[0]) this->mMins[0] = (*itr).x();
		if ((*itr).y() < this->mMins[1]) this->mMins[1] = (*itr).y();
		if ((*itr).z() < this->mMins[2]) this->mMins[2] = (*itr).z();
	}
}

void Brush::move(float x, float y, float z)
{
	for (std::vector<Vector3>::iterator itr = this->mVertices.begin(); itr != this->mVertices.end(); ++itr)
	{
		(*itr).x((*itr).x()+x);
		(*itr).y((*itr).y()+y);
		(*itr).z((*itr).z()+z);
	}
	for (std::vector<Plane>::iterator itr = this->mPlanes.begin(); itr != this->mPlanes.end(); ++itr)
	{
		(*itr).updateFromVertices(this);
	}
	this->updateBounds();
}

void Brush::scale(float x, float y, float z, const Vector3& origin)
{
	for (std::vector<Vector3>::iterator itr = this->mVertices.begin(); itr != this->mVertices.end(); ++itr)
	{
		float newx = (((*itr).x() - origin.x()) * x) + origin.x();
		float newy = (((*itr).y() - origin.y()) * y) + origin.y();
		float newz = (((*itr).z() - origin.z()) * z) + origin.z();
		(*itr).x(newx);
		(*itr).y(newy);
		(*itr).z(newz);
	}
	for (std::vector<Plane>::iterator itr = this->mPlanes.begin(); itr != this->mPlanes.end(); ++itr)
	{
		(*itr).updateFromVertices(this);
	}
	this->updateBounds();
}

void Brush::rotate(float x, float y, float z, const Vector3& origin)
{
	for (std::vector<Vector3>::iterator itr = this->mVertices.begin(); itr != this->mVertices.end(); ++itr)
	{
		float resultx = (*itr).x()-origin.x();
		float resulty = (*itr).y()-origin.y();
		float resultz = (*itr).z()-origin.z();

		// ToDo rotate here
		
		(*itr).x(resultx+origin.x());
		(*itr).y(resulty+origin.y());
		(*itr).z(resultz+origin.z());
	}
	for (std::vector<Plane>::iterator itr = this->mPlanes.begin(); itr != this->mPlanes.end(); ++itr)
	{
		(*itr).updateFromVertices(this);
	}
	this->updateBounds();
}

Vector3 Brush::origin()
{
	return Vector3(
			this->mMins[0] + ((this->mMaxs[0]-this->mMins[0]) / 2),
			this->mMins[1] + ((this->mMaxs[1]-this->mMins[1]) / 2),
			this->mMins[2] + ((this->mMaxs[2]-this->mMins[2]) / 2)
		);
}

}
