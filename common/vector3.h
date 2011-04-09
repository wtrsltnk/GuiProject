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

#ifndef _VECTOR3_H
#define	_VECTOR3_H

enum {
  X = 0, Y = 1, Z = 2, W = 3
};

/*! 
 * \brief
 */
class Vector3
{
public:
    Vector3();
    Vector3(const Vector3& v);
    Vector3(float x, float y, float z);
    Vector3(const float v[3]);
    virtual ~Vector3();

    /// The magnitude or length of this vector. It is calculated with (x^2, y^2, z^2)^.5
    float length() const;

    /// Returns the unit vector with the same direction as this vector. It is normalized by dividing the vector with it's length
    const Vector3 unit() const;

    /// Returns the cross product. Calculated with (this)x(V)
    const Vector3 crossProduct(const Vector3& v) const;

    /// Returns the dotproduct. Calculated with (this).(V), this.x*v.x + this.y*v.y + this.z*v.z
    float dotProduct(const Vector3& v) const;

    /// Inverses the vector
    void inverseMe();

    /// Returns the vector as a float array
    operator float* ();

    /// Returns the vector as a constant float array
    operator const float* () const;

    /// Sets this vector to the v vector and returns a constant reference
    const Vector3& operator = (const Vector3& v);

    /// Sets this vector to the v array with floats and returns a constant reference
    const Vector3& operator = (const float v[3]);

    /// The equal operator for two vectors
    bool operator == (const Vector3& v) const;

    /// The not equal operator for two vectors
    bool operator != (const Vector3& v) const;

    /// The equal operator for this vector and a float array
    bool operator == (const float v[3]) const;

    /// The not equal operator for this vector and a float array
    bool operator != (const float v[3]) const;

    /// The operator for getting the result of multiplying this vector with another vector
    Vector3 operator * (const Vector3& v) const;

    /// The operator for multiplying this vector with another vector
    const Vector3& operator *= (const Vector3& v);

    /// The operator for getting the result of adding another vector to this vector
    Vector3 operator + (const Vector3& v) const;

    /// The operator for adding another vector to this vector
    const Vector3& operator += (const Vector3& v);

    /// The operator for getting the result of subtracting another vector to this vector
    Vector3 operator - (const Vector3& v) const;

    /// The operator for subtracting another vector to this vector
    const Vector3& operator -= (const Vector3& v);

    /// The operator for getting the result of multiplying this vector with a scalar
    Vector3 operator * (float s) const;

    /// The operator for multiplying this vector with a scalar
    const Vector3& operator *= (float s);

    /// The operator for getting the result of adding a scalar to this vector
    Vector3 operator + (float s) const;

    /// The operator for adding a scalar to this vector
    const Vector3& operator += (float s);

    /// Returns the x value of the vector
    float x() const { return mVector[0]; }

    /// Sets the x value of the vector
    void x(float x) { mVector[0] = x; }

    /// Returns the y value of the vector
    float y() const { return mVector[1]; }

    /// Sets the y value of the vector
    void y(float y) { mVector[1] = y; }

    /// Returns the z value of the vector
    float z() const { return mVector[2]; }

    /// Sets the z value of the vector
    void z(float z) { mVector[2] = z; }

    /// Copies the vector to the given float array
    float* copyTo(float f[3]) const;

    const char* toString(char string[]) const;
	void print() const;

private:
    /*! \brief */
    float mVector[3];

    friend Vector3 operator * (float s, const Vector3& v);
    friend Vector3 operator + (float s, const Vector3& v);
    friend bool operator == (const float f[3], const Vector3& v);
    friend bool operator != (const float f[3], const Vector3& v);
	
};

/// The public operator for getting the result of multiplying a scalar with a vector
Vector3 operator * (float s, const Vector3& v);

/// The public operator for getting the result of adding a scalar to a vector
Vector3 operator + (float s, const Vector3& v);

/// The public equal operator for this vector and a float array
bool operator == (const float f[3], const Vector3& v);

/// The public not equal operator for this vector and a float array
bool operator != (const float f[3], const Vector3& v);

#endif	/* _VECTOR3_H */

