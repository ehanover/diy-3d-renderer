#pragma once

#include "mymatrix.h"
#include "myvector.h"

class Camera {

public:
	Camera();

	void moveCamera(MyVector newEye, MyVector newAt, MyVector newUp);

	MyMatrix projectionMatrix() const;

private:
	void setProjectionMatrix();

	MyVector mCameraEye;
	MyVector mCameraAt;
	MyVector mCameraUp;

	MyMatrix mProjectionMatrix;

};
