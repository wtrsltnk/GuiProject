#ifndef _CAMERA_H
#define	_CAMERA_H

#include "common/matrix4x4.h"
#include "common/vector3.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

    void update();

    void setRotation(float rotation[3]);
    void setRotation(float x, float y, float z);
	Vector3& rotation();
	void rotate(float x, float y, float z);
	void rotateX(float amount);
	void rotateY(float amount);
	void rotateZ(float amount);

    void setPosition(const float position[3]);
    void setPosition(float x, float y, float z);
	Vector3& position();
    void move(float forward, float left, float up);
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

