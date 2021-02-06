#pragma once

#include "myvector.h"

class Light {

public:
	Light(double x, double y, double z);

	const MyVector& position() const;

private:
	MyVector mPosition;

};

inline const MyVector& Light::position() const {
	return mPosition;
}
