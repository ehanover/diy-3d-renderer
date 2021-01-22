#include "camera.h"

Camera::Camera() :
	mCameraEye(MyVector(std::vector<double>{0, 1, 4})),
	mCameraAt(MyVector(std::vector<double>{0, 0, 0})),
	mCameraUp(MyVector(std::vector<double>{0, 1, 0})),
	mProjectionMatrix()
{
	setProjectionMatrix();
}

void Camera::setProjectionMatrix() {
	// MyVector camAxisZ(mCameraEye);
	// camAxisZ.scalar(-1).add(mCameraAt).normalize();
	// MyVector camAxisX(mCameraUp);
	// camAxisX.cross(camAxisZ).normalize();
	// MyVector camAxisY(camAxisZ);
	// camAxisY.cross(camAxisX).scalar(-1).normalize();
	MyVector camAxisZ(mCameraAt);
	camAxisZ.scalar(-1).add(mCameraEye).normalize();
	MyVector camAxisX(mCameraUp);
	camAxisX.cross(camAxisZ).scalar(-1).normalize();
	MyVector camAxisY(camAxisX);
	camAxisY.cross(camAxisZ).normalize();
	// std::cout << "camAxisX=" << camAxisX << std::endl;
	// std::cout << "camAxisY=" << camAxisY << std::endl;
	// std::cout << "camAxisZ=" << camAxisZ << std::endl;

	std::vector<double> projectionVec{
		camAxisX.elem(0), camAxisY.elem(0), camAxisZ.elem(0), 0, 
		camAxisX.elem(1), camAxisY.elem(1), camAxisZ.elem(1), 0, 
		camAxisX.elem(2), camAxisY.elem(2), camAxisZ.elem(2), 0, 
		-1*(camAxisX.dot(mCameraEye)), -1*(camAxisY.dot(mCameraEye)), -1*(camAxisZ.dot(mCameraEye)), 1
	};
	// https://www.geertarien.com/blog/2017/07/30/breakdown-of-the-lookAt-function-in-OpenGL/
	// std::vector<double> projectionVec{
	// 	camAxisX.elem(0), camAxisX.elem(1), camAxisX.elem(2), -1*(camAxisX.dot(mCameraEye)), 
	// 	camAxisY.elem(0), camAxisY.elem(1), camAxisY.elem(2), -1*(camAxisY.dot(mCameraEye)), 
	// 	camAxisZ.elem(0), camAxisZ.elem(1), camAxisZ.elem(2), -1*(camAxisZ.dot(mCameraEye)), 
	// 	0, 0, 0, 1
	// };

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
