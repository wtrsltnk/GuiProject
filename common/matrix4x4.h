#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "vector3.h"

class Quaternion;

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(float (*matrix)[4]);
	virtual ~Matrix4x4();

	void print();

	const Vector3 forwardVector() const;
	const Vector3 leftVector() const;
	const Vector3 upVector() const;

	void glMultiply();

	Matrix4x4 operator * (const Matrix4x4& other);
	
public:
	static void multiplyMatrix(float first[16], float second[16], float out[16]);
	static Matrix4x4 rotateMatrixX(float angle);
	static Matrix4x4 rotateMatrixY(float angle);
	static Matrix4x4 rotateMatrixZ(float angle);
	static Matrix4x4 translateMatrix(float x, float y, float z);

private:
	float m[4][4];

public:
	(*operator float (void))[4] { return this->m; }

};

#endif // MATRIX4X4_H
