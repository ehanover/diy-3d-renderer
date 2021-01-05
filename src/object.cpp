#include "object.h"

Object::Object(std::vector<MyVector> vs, std::vector<std::array<size_t, 3>> ts) :
	mPosition(MyVector(std::vector<double>{0,0,0})),
	mRotation(MyVector(std::vector<double>{0,0,0})),
	mScale(MyVector(std::vector<double>{1,1,1})),
	mVerts(vs), mTris(ts), mNorms()
{
	calculateNormals();
}

Object::Object(std::vector<MyVector> vs, std::vector<std::array<size_t, 3>> ts, std::vector<MyVector> ns) :
	mPosition(MyVector(std::vector<double>{0,0,0})),
	mRotation(MyVector(std::vector<double>{0,0,0})),
	mScale(MyVector(std::vector<double>{1,1,1})),
	mVerts(vs), mTris(ts), mNorms(ns)
{

}

Object::Object(const Object& o) :
	mPosition(o.position()),
	mRotation(o.rotation()),
	mScale(o.scale()),
	mVerts(o.verts()), mTris(o.tris()), mNorms(o.norms())
{

}

std::vector<MyVector> Object::verts() const {
	return mVerts;
}

std::vector<std::array<size_t, 3>> Object::tris() const {
	return mTris;
}

std::vector<MyVector> Object::norms() const {
	return mNorms;
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
	mPosition = p;
}

void Object::setRotation(MyVector r) {
	mRotation = r;
}

void Object::setScale(MyVector s) {
	mScale = s;
}

void Object::calculateNormals() {
	std::vector<MyVector> normsVec;
	normsVec.reserve(tris().size());
	for(size_t i=0; i<tris().size(); i++) {
		std::array<size_t, 3> tri = tris().at(i);
		std::array<double, 3> triVert1 = {mVerts.at(tri[0]).elem(0), mVerts.at(tri[0]).elem(1), mVerts.at(tri[0]).elem(2)};
		std::array<double, 3> triVert2 = {mVerts.at(tri[1]).elem(0), mVerts.at(tri[1]).elem(1), mVerts.at(tri[1]).elem(2)};
		std::array<double, 3> triVert3 = {mVerts.at(tri[2]).elem(0), mVerts.at(tri[2]).elem(1), mVerts.at(tri[2]).elem(2)};

		MyVector a = MyVector(std::vector<double>{triVert2[0] - triVert1[0], triVert2[1] - triVert1[1], triVert2[2] - triVert1[2]});
		MyVector b = MyVector(std::vector<double>{triVert3[0] - triVert1[0], triVert3[1] - triVert1[1], triVert3[2] - triVert1[2]});
		// a.cross(b);
		// a.normalize();
		// mNorms.push_back(a);
		b.cross(a);
		b.normalize();
		mNorms.push_back(b);
	}
}
