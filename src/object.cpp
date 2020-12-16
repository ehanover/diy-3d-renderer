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

MyMatrix Object::verts() const {
	return mVerts;
}

/*MyMatrix Object::vertsMat() const {
	std::vector<double> v(mVerts.size() * 4);
	for(size_t i=0; i<mVerts.size(); i++) {
		for(size_t j=0; j<4; j++) {
			v[i*4 + j] = mVerts.at(i).at(j);
		}
	}
	// std::cout << "done with vertsMat" << std::endl;
	return MyMatrix(mVerts.size(), 4, v);
}*/

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
