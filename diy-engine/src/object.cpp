#include "object.h"

Object::Object(std::vector<MyVector> vs, std::vector<std::array<size_t, 3>> ts) :
	mPosition(std::array<double, 3>{0, 0, 0}),
	mRotation(std::array<double, 3>{0, 0, 0}),
	mScale(std::array<double, 3>{1, 1, 1}),
	mColor({255, 0, 255}),
	mVerts(vs), mTris(ts), mNorms()
{
	calculateNormals();
}

Object::Object(std::vector<MyVector> vs, std::vector<std::array<size_t, 3>> ts, std::vector<MyVector> ns) :
	mPosition(std::array<double, 3>{0,0,0}),
	mRotation(std::array<double, 3>{0,0,0}),
	mScale(std::array<double, 3>{1,1,1}),
	mColor({255, 0, 255}),
	mVerts(vs), mTris(ts), mNorms(ns)
{
	if(mNorms.size() == 0 || mNorms.size() < mTris.size()) {
		mNorms.clear();
		calculateNormals();
	}
}

Object::Object(const Object& o) :
	mPosition(o.position()),
	mRotation(o.rotation()),
	mScale(o.scale()),
	mColor(o.color()),
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

std::array<double, 3> Object::position() const {
	return mPosition;
}

std::array<double, 3> Object::rotation() const {
	return mRotation;
}

std::array<double, 3> Object::scale() const {
	return mScale;
}

void Object::setPosition(std::array<double, 3> p) {
	mPosition = p;
}

void Object::setRotation(std::array<double, 3> r) {
	mRotation = r;
}

void Object::setScale(std::array<double, 3> s) {
	mScale = s;
}

std::array<uint8_t, 3> Object::color() const {
	return mColor;
}

void Object::setColor(std::array<uint8_t, 3> c) {
	mColor = c;
}

void Object::calculateNormals() {
	// std::cout << "manually calculating normals" << std::endl;
	std::vector<MyVector> normsVec;
	normsVec.reserve(tris().size());
	for(size_t i=0; i<tris().size(); i++) {
		std::array<size_t, 3> tri = tris().at(i);
		std::array<double, 3> triVert1 = {mVerts.at(tri[0]).elem(0), mVerts.at(tri[0]).elem(1), mVerts.at(tri[0]).elem(2)};
		std::array<double, 3> triVert2 = {mVerts.at(tri[1]).elem(0), mVerts.at(tri[1]).elem(1), mVerts.at(tri[1]).elem(2)};
		std::array<double, 3> triVert3 = {mVerts.at(tri[2]).elem(0), mVerts.at(tri[2]).elem(1), mVerts.at(tri[2]).elem(2)};

		MyVector a = MyVector(std::vector<double>{triVert2[0] - triVert1[0], triVert2[1] - triVert1[1], triVert2[2] - triVert1[2]});
		MyVector b = MyVector(std::vector<double>{triVert3[0] - triVert1[0], triVert3[1] - triVert1[1], triVert3[2] - triVert1[2]});
		a.cross(b).normalize();
		mNorms.push_back(a);
		// b.cross(a).normalize();
		// mNorms.push_back(b);
	}
}
