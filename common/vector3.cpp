/*
 * Copyright (C) 2009  Wouter Saaltink

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vector3.h"
#include <math.h>
#include <stdio.h>

/*!
 * \brief
 */
Vector3::Vector3()
{
    this->mVector[0] = 0.0f;
    this->mVector[1] = 0.0f;
    this->mVector[2] = 0.0f;
}

/*!
 * \brief
 * \param x
 * \param y
 * \param z
 */
Vector3::Vector3(float x, float y, float z)
{
    this->mVector[0] = x;
    this->mVector[1] = y;
    this->mVector[2] = z;
}

/*!
 * \brief
 * \param v
 */
Vector3::Vector3(const Vector3& v)
{
    (*this) = v;
}

/*!
 * \brief
 * \param v
 */
Vector3::Vector3(const float v[3])
{
    (*this) = v;
}

/*!
 * \brief
 */
Vector3::~Vector3()
{
}

/*!
 * \brief
 * \return
 */
float Vector3::length() const
{
    return sqrtf(this->mVector[0] * this->mVector[0] + this->mVector[1] * this->mVector[1] + this->mVector[2] * this->mVector[2]);
}

/*!
 * \brief
 * \return
 */
const Vector3 Vector3::unit() const
{
    float length = this->length();

    if (length == 0)
        return Vector3(this->mVector[0], this->mVector[1], this->mVector[2]);
    
    return Vector3(this->mVector[0] / length, this->mVector[1] / length, this->mVector[2] / length);
}

/*!
 * \brief
 * \param v
 * \return
 */
const Vector3 Vector3::crossProduct(const Vector3& v) const
{
	return Vector3(
			mVector[1] * v.mVector[2] - mVector[2] * v.mVector[1],
			mVector[2] * v.mVector[0] - mVector[0] * v.mVector[2],
			mVector[0] * v.mVector[1] - mVector[1] * v.mVector[0]
		);
}

/*!
 * \brief
 * \param v
 * \return
 */
float Vector3::dotProduct(const Vector3& v) const
{
    return (this->mVector[0] * v.mVector[0]) + (this->mVector[1] * v.mVector[1]) + (this->mVector[2] * v.mVector[2]);
}

/*!
 * \brief
 */
void  Vector3::inverseMe()
{
    this->mVector[0] = -this->mVector[0];
    this->mVector[1] = -this->mVector[1];
    this->mVector[2] = -this->mVector[2];
}

/*!
 * \brief
 * \return
 */
Vector3::operator float* ()
{
    return this->mVector;
}

/*!
 * \brief
 * \return
 */
Vector3::operator const float* () const
{
    return this->mVector;
}

/*!
 * \brief
 * \param v
 * \return
 */
const Vector3& Vector3::operator = (const Vector3& v)
{
    for (int i = 0; i < 3; i++)
        this->mVector[i] = v.mVector[i];

    return (*this);
}

/*!
 * \brief
 * \param v
 * \return
 */
const Vector3& Vector3::operator = (const float v[3])
{
    for (int i = 0; i < 3; i++)
        this->mVector[i] = v[i];

    return (*this);
}

/*!
 * \brief
 * \param v
 * \return
 */
bool Vector3::operator == (const Vector3& v) const
{
    return ((this->mVector[0] == v.mVector[0]) && (this->mVector[1] == v.mVector[1]) && (this->mVector[2] == v.mVector[2]));
}

/*!
 * \brief
 * \param v
 * \return
 */
bool Vector3::operator != (const Vector3& v) const
{
    return !((*this) == v);
}

/*!
 * \brief
 * \param v
 * \return
 */
bool Vector3::operator == (const float v[3]) const
{
    return (this->mVector[0] == v[0] && this->mVector[1] == v[1] && this->mVector[2] == v[2]);
}

/*!
 * \brief
 * \param v
 * \return
 */
bool Vector3::operator != (const float v[3]) const
{
    return (this->mVector[0] != v[0] || this->mVector[1] != v[1] || this->mVector[2] != v[2]);
}

/*!
 * \brief
 * \param v
 * \return
 */
Vector3 Vector3::operator * (const Vector3& v) const
{
    return Vector3(this->mVector[0] * v.mVector[0], this->mVector[1] * v.mVector[1], this->mVector[2] * v.mVector[2]);
}

/*!
 * \brief
 * \param v
 * \return
 */
const Vector3& Vector3::operator *= (const Vector3& v)
{
    this->mVector[0] *= v.mVector[0];
    this->mVector[1] *= v.mVector[1];
    this->mVector[2] *= v.mVector[2];

    return (*this);
}

/*!
 * \brief
 * \param v
 * \return
 */
Vector3 Vector3::operator + (const Vector3& v) const
{
    return Vector3(this->mVector[0] + v.mVector[0], this->mVector[1] + v.mVector[1], this->mVector[2] + v.mVector[2]);
}

/*!
 * \brief
 * \param v
 * \return
 */
const Vector3& Vector3::operator += (const Vector3& v)
{
    this->mVector[0] += v.mVector[0];
    this->mVector[1] += v.mVector[1];
    this->mVector[2] += v.mVector[2];

    return (*this);
}

/*!
 * \brief
 * \param v
 * \return
 */
Vector3 Vector3::operator - (const Vector3& v) const
{
    return Vector3(this->mVector[0] - v.mVector[0], this->mVector[1] - v.mVector[1], this->mVector[2] - v.mVector[2]);
}

/*!
 * \brief
 * \param v
 * \return
 */
const Vector3& Vector3::operator -= (const Vector3& v)
{
    this->mVector[0] -= v.mVector[0];
    this->mVector[1] -= v.mVector[1];
    this->mVector[2] -= v.mVector[2];

    return (*this);
}

/*!
 * \brief
 * \param s
 * \return
 */
Vector3 Vector3::operator * (float s) const
{
    return Vector3(this->mVector[0] * s, this->mVector[1] * s, this->mVector[2] * s);
}

/*!
 * \brief
 * \param s
 * \return
 */
const Vector3& Vector3::operator *= (float s)
{
    this->mVector[0] *= s;
    this->mVector[1] *= s;
    this->mVector[2] *= s;

    return (*this);
}

/*!
 * \brief
 * \param s
 * \return
 */
Vector3 Vector3::operator + (float s) const
{
    return Vector3(this->mVector[0] + s, this->mVector[1] + s, this->mVector[2] + s);
}

/*!
 * \brief
 * \param s
 * \return
 */
const Vector3& Vector3::operator += (float s)
{
    this->mVector[0] += s;
    this->mVector[1] += s;
    this->mVector[2] += s;

    return (*this);
}

/*!
 * \brief
 * \param f
 * \return
 */
float* Vector3::copyTo(float f[3]) const
{
    f[0] = this->mVector[0];
    f[1] = this->mVector[1];
    f[2] = this->mVector[2];

    return f;
}

/*!
 * \brief
 * \param string
 * \return
 */
const char* Vector3::toString(char string[]) const
{
    sprintf(string, " v=(%f %f %f)", this->mVector[0], this->mVector[1], this->mVector[2]);
    return string;
}

void Vector3::print() const
{
	printf("v=(%f %f %f)\n", this->mVector[0], this->mVector[1], this->mVector[2]);
}

/*!
 * \brief
 * \param s
 * \param v
 * \return
 */
Vector3 operator * (float s, const Vector3& v)
{
    return Vector3(v.mVector[0] * s, v.mVector[1] * s, v.mVector[2] * s);
}

/*!
 * \brief
 * \param s
 * \param v
 * \return
 */
Vector3 operator + (float s, const Vector3& v)
{
    return Vector3(v.mVector[0] + s, v.mVector[1] + s, v.mVector[2] + s);
}

/*!
 * \brief
 * \param f
 * \param v
 * \return
 */
bool operator == (const float f[3], const Vector3& v)
{
    return (f[0] == v.mVector[0] && f[1] == v.mVector[1] && f[2] == v.mVector[2]);
}

/*!
 * \brief
 * \param f
 * \param v
 * \return
 */
bool operator != (const float f[3], const Vector3& v)
{
    return (f[0] != v.mVector[0] || f[1] != v.mVector[1] || f[2] != v.mVector[2]);
}
