#include "camera.h"

Camera::Camera() :
	mCameraEye(MyVector(std::vector<double>{0, 0, 5})),
	mCameraAt(MyVector(std::vector<double>{0, 0, 0})),
	mCameraUp(MyVector(std::vector<double>{0, 1, 0})),
	mProjectionMatrix()
{
	setProjectionMatrix();
}

Camera::Camera(double ex, double ey, double ez, double ax, double ay, double az, double ux, double uy, double uz) :
	mCameraEye(MyVector(std::vector<double>{ex, ey, ez})),
	mCameraAt(MyVector(std::vector<double>{ax, ay, az})),
	mCameraUp(MyVector(std::vector<double>{ux, uy, uz})),
	mProjectionMatrix()
{
	setProjectionMatrix();
}

void Camera::setProjectionMatrix() {
	MyVector camAxisZ(mCameraAt);
	camAxisZ.scalar(-1).add(mCameraEye).normalize();
	MyVector camAxisX(mCameraUp);
	camAxisX.cross(camAxisZ).scalar(-1).normalize();
	MyVector camAxisY(camAxisX);
	camAxisY.cross(camAxisZ).normalize();

	std::vector<double> projectionVec{
		camAxisX.elem(0), camAxisY.elem(0), camAxisZ.elem(0), 0, 
		camAxisX.elem(1), camAxisY.elem(1), camAxisZ.elem(1), 0, 
		camAxisX.elem(2), camAxisY.elem(2), camAxisZ.elem(2), 0, 
		-1*(camAxisX.dot(mCameraEye)), -1*(camAxisY.dot(mCameraEye)), -1*(camAxisZ.dot(mCameraEye)), 1
	};
	mProjectionMatrix = MyMatrix(4, 4, projectionVec);
}

void Camera::moveCamera(MyVector newEye, MyVector newAt, MyVector newUp) {
	mCameraEye = newEye;
	mCameraAt = newAt;
	mCameraUp = newUp;
	setProjectionMatrix();
}

MyVector Camera::cameraEye() const {
	return mCameraEye;
}

MyMatrix Camera::projectionMatrix() const {
	return mProjectionMatrix;
}
