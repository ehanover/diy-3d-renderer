#include "object.h"
#include <array>
#include <vector>

Object::Object(std::vector<std::array<double, 4>> vs, std::vector<std::array<size_t, 3>> ts) :
	mVerts(vs), mTris(ts), 
	mPosition(MyVector(std::vector<double>{0,0,0})),
	mScale(MyVector(std::vector<double>{1,1,1}))
{
	
}

std::vector<std::array<double, 4>> Object::verts() const {
	return mVerts;
}

std::vector<std::array<size_t, 3>> Object::tris() const {
	return mTris;
}

MyVector Object::position() const {
	return mPosition;
}

MyVector Object::scale() const {
	return mScale;
}

void Object::setPosition(MyVector d) {
	// TODO add vector inplace modifiers
}

void Object::setScale(MyVector s) {

}
