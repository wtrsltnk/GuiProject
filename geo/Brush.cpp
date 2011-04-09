/* 
 * File:   Brush.cpp
 * Author: wouter
 * 
 * Created on April 1, 2011, 10:56 PM
 */

#include "Brush.h"
#include <cmath>
#include <map>
#include <stdio.h>

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
	this->mColor[0] = 0;
	this->mColor[1] = 0;
	this->mColor[2] = 0;
	this->mColor[brushCount%3] = (155 + (brushCount*10) % 99) / 255.0f;
	this->mColor[(brushCount+1)%3] = (155 + (brushCount*7) % 99) / 255.0f;
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

float calculateSignedAngle(const Vector3& v1, const Vector3& v2, const Vector3& normal)
{
	Vector3 c = v2.crossProduct(v1);
	float angle = std::atan2(double((v2-v1).length()), double(v2.dotProduct(v1)));
	return c.dotProduct(normal) < float(0) ? -angle : angle;
}

int Brush::getIndexOf(const Vector3& vertex)
{
	for (int i = 0; i < this->mVertices.size(); i++)
		if (this->mVertices[i] == vertex)
			return i;
	return -1;
}

void Brush::updateVertices()
{
	for(int i = 0; i < this->mPlanes.size() - 2; i++)
	{
		for(int j = 0; j < this->mPlanes.size() - 1; j++)
		{
			for(int k = 0; k < this->mPlanes.size(); k++)
			{
				if(i != j && i != k && j != k)
				{
					Vector3 intersection;
					if (Plane::getIntersection(this->mPlanes[i], this->mPlanes[j], this->mPlanes[k], intersection) && Brush::pointInWorld(intersection))
					{
						bool bLegal = true;

						for(int l = 0; l < this->mPlanes.size(); l++)
						{
							if(l != i && l != j && l != k)
							{
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
							int index = this->getIndexOf(intersection);
							if (index == -1)
							{
								index = this->mVertices.size();
								this->mVertices.push_back(intersection);
							}
							if (this->mPlanes[i].getIndexOf(index) == -1) this->mPlanes[i].mIndices.push_back(index);
							if (this->mPlanes[j].getIndexOf(index) == -1) this->mPlanes[j].mIndices.push_back(index);
							if (this->mPlanes[k].getIndexOf(index) == -1) this->mPlanes[k].mIndices.push_back(index);
						}
					}
				}
			}
		}
	}
	for(int i = 0; i < this->mPlanes.size(); i++)
	{
		std::map<float, int> indices;
		
		for(int j = 0; j < this->mPlanes[i].mIndices.size(); j++)
			this->mPlanes[i].average += this->mVertices[this->mPlanes[i].mIndices[j]];
		this->mPlanes[i].average *= (1.0f / this->mPlanes[i].mIndices.size());

		if (this->mPlanes[i].mIndices.size() > 0)
		{
			Vector3 start = this->mVertices[this->mPlanes[i].mIndices[0]];
			indices.insert(std::make_pair(0, this->mPlanes[i].mIndices[0]));
			for (int j = 1; j < this->mPlanes[i].mIndices.size(); j++)
			{
				float angle = calculateSignedAngle(start-this->mPlanes[i].average, this->mVertices[this->mPlanes[i].mIndices[j]]-this->mPlanes[i].average, this->mPlanes[i].mNormal);
				indices.insert(std::make_pair(angle, this->mPlanes[i].mIndices[j]));
			}
			this->mPlanes[i].mIndices.clear();
			for (std::map<float, int>::iterator itr = indices.begin(); itr != indices.end(); ++itr)
			{
				this->mPlanes[i].mIndices.push_back(itr->second);
			}
		}
	}
}

}
