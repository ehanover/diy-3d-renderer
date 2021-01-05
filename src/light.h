#pragma once

#include "myvector.h"

class Light {

public:
	Light(MyVector p);

	MyVector position() const;

private:
	MyVector mPosition;
	// double mIntensity;

};
