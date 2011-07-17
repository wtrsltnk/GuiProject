#include "camera.h"

#define PI 3.14159265
#define Deg2Rad(Ang) ((float)( Ang * PI / 180.0 ))

Camera::Camera()
	: mIsChanged(true)
{
}

Camera::~Camera()
{
}

void Camera::update()
{
	(Matrix4x4::translateMatrix(this->mPosition[0], this->mPosition[1], this->mPosition[2]) * this->mMatrix).glMultiply();
}

void Camera::setRotation(float r[3])
{
	if (r[0] != 0 || r[1] != 0 || r[2] != 0) this->mIsChanged = true;

	this->setRotation(r[0], r[1], r[2]);
}

void Camera::setRotation(float x, float y, float z)
{
	if (x != 0 || y != 0 || z != 0) this->mIsChanged = true;

	this->mRotation = Vector3(x, y, z);
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

Vector3& Camera::rotation()
{
	return this->mRotation;
}

void Camera::rotate(float x, float y, float z)
{
	if (x != 0 || y != 0 || z != 0) this->mIsChanged = true;

	this->mRotation[0] += x;
	this->mRotation[1] += y;
	this->mRotation[2] += z;
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateX(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	this->mRotation[0] += amount;
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateY(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	this->mRotation[1] += amount;
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateZ(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	this->mRotation[2] += amount;
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateDegree(float x, float y, float z)
{
	if (x != 0 || y != 0 || z != 0) this->mIsChanged = true;

	this->mRotation[0] += Deg2Rad(x);
	this->mRotation[1] += Deg2Rad(y);
	this->mRotation[2] += Deg2Rad(z);
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateXDegree(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	this->mRotation[0] += Deg2Rad(amount);
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateYDegree(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	this->mRotation[1] += Deg2Rad(amount);
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::rotateZDegree(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	this->mRotation[2] += Deg2Rad(amount);
    this->mMatrix = Matrix4x4::rotateMatrixZ(this->mRotation[2]) * Matrix4x4::rotateMatrixY(this->mRotation[1]) * Matrix4x4::rotateMatrixX(this->mRotation[0]);
}

void Camera::setPosition(const float mPosition[3])
{
	this->setPosition(this->mPosition[0], this->mPosition[1], this->mPosition[2]);
}

void Camera::setPosition(float x, float y, float z)
{
	if (x != 0 || y != 0 || z != 0) this->mIsChanged = true;

	this->mPosition = Vector3(x, y, z);
}

Vector3& Camera::position()
{
	return this->mPosition;
}

void Camera::move(float forward, float left, float up)
{
//	if (forward != 0 || left != 0 || up != 0) this->mIsChanged = true;
//
//    this->mPosition += (this->mMatrix.forwardVector() * forward) * (this->mMatrix.leftVector() * left) * (this->mMatrix.upVector() * up);

	setPosition(this->mPosition.x()+forward, this->mPosition.y()+left, this->mPosition.z()+up);
}

void Camera::moveForward(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	setPosition(this->mPosition.x()+(this->mMatrix.forwardVector() * amount).x(),
			this->mPosition.y()+(this->mMatrix.forwardVector() * amount).y(),
			this->mPosition.z()+(this->mMatrix.forwardVector() * amount).z());
}

void Camera::moveUp(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	setPosition(this->mPosition.x()+(this->mMatrix.upVector() * amount).x(),
			this->mPosition.y()+(this->mMatrix.upVector() * amount).y(),
			this->mPosition.z()+(this->mMatrix.upVector() * amount).z());
}

void Camera::moveLeft(float amount)
{
	if (amount != 0) this->mIsChanged = true;

	setPosition(this->mPosition.x()+(this->mMatrix.leftVector() * amount).x(),
			this->mPosition.y()+(this->mMatrix.leftVector() * amount).y(),
			this->mPosition.z()+(this->mMatrix.leftVector() * amount).z());
}

Vector3 Camera::forward()
{
	return this->mMatrix.forwardVector();
}

Vector3 Camera::up()
{
	return this->mMatrix.upVector();
}

Vector3 Camera::left()
{
	return this->mMatrix.leftVector();
}

bool Camera::isChanged()
{
	bool tmp = this->mIsChanged;

	this->mIsChanged = false;
	
	return tmp;
}
