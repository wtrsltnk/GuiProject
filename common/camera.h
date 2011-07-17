#ifndef _CAMERA_H
#define	_CAMERA_H

#include "matrix4x4.h"
#include "vector3.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

    void update();

	Vector3& rotation();
    void setRotation(float rotation[3]);
    virtual void setRotation(float x, float y, float z);
	void rotate(float x, float y, float z);
	void rotateX(float amount);
	void rotateY(float amount);
	void rotateZ(float amount);
	void rotateDegree(float x, float y, float z);
	void rotateXDegree(float amount);
	void rotateYDegree(float amount);
	void rotateZDegree(float amount);

	Vector3& position();
    void setPosition(const float position[3]);
    virtual void setPosition(float x, float y, float z);
    virtual void move(float forward, float left, float up);
	void moveForward(float amount);
	void moveUp(float amount);
	void moveLeft(float amount);

	Vector3 forward();
	Vector3 up();
	Vector3 left();

	bool isChanged();

private:
    Matrix4x4 mMatrix;
	Vector3 mRotation;
    Vector3 mPosition;
	bool mIsChanged;

};

#endif	/* _CAMERA_H */

