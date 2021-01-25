#pragma once

#include "mymatrix.h"
#include "myvector.h"

class Camera {

public:
	Camera();
	Camera(double ex, double ey, double ez, double ax, double ay, double az, double ux, double uy, double uz);

	void moveCamera(MyVector newEye, MyVector newAt, MyVector newUp);

	MyVector cameraEye() const;

	MyMatrix projectionMatrix() const;

private:
	void setProjectionMatrix();

	MyVector mCameraEye;
	MyVector mCameraAt;
	MyVector mCameraUp;

	MyMatrix mProjectionMatrix;

};
