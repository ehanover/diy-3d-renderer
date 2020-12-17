#include "object.h"
#include <array>
#include <vector>

Object::Object(MyMatrix vs, std::vector<std::array<size_t, 3>> ts) :
	mVerts(vs), mTris(ts),
	mPosition(MyVector(std::vector<double>{0,0,0})),
	mRotation(MyVector(std::vector<double>{0,0,0})),
	mScale(MyVector(std::vector<double>{1,1,1}))
{
	
}

Object::Object(const Object& o) :
	mVerts(o.verts()), mTris(o.tris()),
	mPosition(o.position()),
	mRotation(o.rotation()),
	mScale(o.scale())
{

}

MyMatrix Object::verts() const {
	return mVerts;
}

std::vector<std::array<size_t, 3>> Object::tris() const {
	return mTris;
}

MyVector Object::position() const {
	return mPosition;
}

MyVector Object::rotation() const {
	return mRotation;
}

MyVector Object::scale() const {
	return mScale;
}

void Object::setVerts(MyMatrix v) { // Could this be a reference?
	mVerts = v;
}

void Object::setPosition(MyVector p) {
	// TODO add vector inplace modifiers
	mPosition = p;
}

void Object::setRotation(MyVector r) {
	mRotation = r;
}

void Object::setScale(MyVector s) {
	mScale = s;
}
