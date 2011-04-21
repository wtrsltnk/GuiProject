/* 
 * File:   Quaternion.h
 * Author: wouter
 *
 * Created on April 9, 2010, 4:30 PM
 */

#ifndef _QUATERNION_H
#define	_QUATERNION_H

#include "vector3.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z);
	Quaternion(float w, const Vector3& v);
	Quaternion(float w, float x, float y, float z);
	Quaternion(float q[4]);
	Quaternion(const Quaternion& orig);
	virtual ~Quaternion();

    /// Returns the x value of the quaternion
    float x() const { return mQuat[X]; }

    /// Sets the x value of the quaternion
    void x(float x) { mQuat[X] = x; }

    /// Returns the y value of the quaternion
    float y() const { return mQuat[Y]; }

    /// Sets the y value of the quaternion
    void y(float y) { mQuat[Y] = y; }

    /// Returns the z value of the quaternion
    float z() const { return mQuat[Z]; }

    /// Sets the z value of the quaternion
    void z(float z) { mQuat[Z] = z; }

    /// Returns the z value of the quaternion
    float w() const { return mQuat[W]; }

    /// Sets the z value of the quaternion
    void w(float w) { mQuat[W] = w; }

    /// Returns the vector value of the quaternion
    Vector3 v() const { return Vector3(mQuat[1], mQuat[2], mQuat[3]); }

    /// Sets the vector value of the quaternion
    void v(const Vector3& v) { mQuat[1] = v.x(); mQuat[2] = v.y(); mQuat[3] = v.z(); }

    /// Copies the vector to the given float array
    float* copyTo(float q[4]) const;

	/// Create a nice looking string of the values of this quaternion
    const char* toString(char string[]) const;

	/// Create a rotation matrix from this quaternion
	float* toMatrix(float m[4][4]);

    /// Sets this quaternion to the quaternion q and returns a constant reference
    const Quaternion& operator = (const Quaternion& q);

	float operator [] (int index) const;
	
	Vector3 rotatePoint(float point[3]);
	Quaternion multiplyVector(const Vector3& v);
	Quaternion multiplyQuaternion(const Quaternion& q);
	Quaternion inverse();
	void normalize();
	float dotProduct(const Quaternion q);
	Quaternion slerp(const Quaternion& qb, float t);

public:
	static Quaternion fromAngleAxis(float angle, const Vector3& axis);
	static Quaternion fromMatrix(float m[4][4]);
	
private:
	float mQuat[4];

};

#endif	/* _QUATERNION_H */

