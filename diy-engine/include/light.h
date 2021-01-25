#pragma once

#include "myvector.h"

class Light {

public:
	Light(double x, double y, double z);

	MyVector position() const;

private:
	MyVector mPosition;
	// double mIntensity;

};
