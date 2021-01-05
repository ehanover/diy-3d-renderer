#include "light.h"

Light::Light(MyVector p) :
	mPosition(p)
	// mIntensity(1)
{

}

MyVector Light::position() const {
	return mPosition;
}