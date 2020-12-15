#include "camera.h"

Camera::Camera() :
	mCameraEye(MyVector(std::vector<double>{2, 3, 3})),
	mCameraAt(MyVector(std::vector<double>{0, 0, 0})),
	mCameraUp(MyVector(std::vector<double>{0, 1, 0})),
	mProjectionMatrix()
{
	setProjectionMatrix();
}

void Camera::setProjectionMatrix() {
	MyVector camAxisZ = mCameraAt.add(mCameraEye.scalar(-1)).normalize(); // TODO not all these normalizes may be necessary
	MyVector camAxisX = mCameraUp.cross(camAxisZ).normalize();
	MyVector camAxisY = camAxisZ.cross(camAxisX).normalize();

	// cout << "camAxisZ=" << camAxisZ << endl;
	// cout << "camAxisX=" << camAxisX << endl;
	// cout << "camAxisY=" << camAxisY << endl;

	std::vector<double> projectionVec{
		camAxisX.elem(0), camAxisY.elem(0), camAxisZ.elem(0), 0, 
		camAxisX.elem(1), camAxisY.elem(1), camAxisZ.elem(1), 0, 
		camAxisX.elem(2), camAxisY.elem(2), camAxisZ.elem(2), 0, 
		-1*(camAxisX.dot(mCameraEye)), -1*(camAxisY.dot(mCameraEye)), -1*(camAxisZ.dot(mCameraEye)), 1
	};
	mProjectionMatrix = MyMatrix(4, 4, projectionVec);
}

MyMatrix Camera::projectionMatrix() const {
	return mProjectionMatrix;
}

void Camera::moveCamera(MyVector newEye, MyVector newAt, MyVector newUp) {
	mCameraEye = newEye;
	mCameraAt = newAt;
	mCameraUp = newUp;
	setProjectionMatrix();
}
