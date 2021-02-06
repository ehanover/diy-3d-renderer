#include "light.h"

Light::Light(double x, double y, double z) :
	mPosition(MyVector({x, y, z}))
{

}

// const MyVector& Light::position() const { // (moved to be inline)
// 	return mPosition;
// }
