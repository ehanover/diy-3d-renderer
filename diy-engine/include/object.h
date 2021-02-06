#pragma once

#include "mymatrix.h"
#include "myvector.h"
#include <array>
#include <cstddef>
#include <vector>

class Object {

public:
	Object(std::vector<MyVector> vs, std::vector<std::array<size_t, 3>> ts);
	Object(std::vector<MyVector> vs, std::vector<std::array<size_t, 3>> ts, std::vector<MyVector> ns);
	Object(const Object& o);

	std::vector<MyVector> verts() const;
	std::vector<std::array<size_t, 3>> tris() const;
	std::vector<MyVector> norms() const;

	std::array<double, 3> position() const; // TODO should these return references?
	std::array<double, 3> rotation() const;
	std::array<double, 3> scale() const;
	void setPosition(std::array<double, 3> p);
	void setRotation(std::array<double, 3> r);
	void setScale(std::array<double, 3> s);

	std::array<uint8_t, 3> color() const;
	void setColor(std::array<uint8_t, 3> c);

private:
	std::array<double, 3> mPosition;
	std::array<double, 3> mRotation;
	std::array<double, 3> mScale;

	std::array<uint8_t, 3> mColor; // R, G, B

	std::vector<MyVector> mVerts;
	std::vector<std::array<size_t, 3>> mTris;
	std::vector<MyVector> mNorms;

	// Calculates initial surface normals when the object is created. After creation, mNormsTransformed
	// are calculated by the renderer class according to the object's transformation
	void calculateNormals();

};
