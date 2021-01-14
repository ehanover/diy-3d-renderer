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

	MyVector position() const;
	MyVector rotation() const;
	MyVector scale() const;
	void setPosition(MyVector p);
	void setRotation(MyVector r);
	void setScale(MyVector s);

private:
	MyVector mPosition;
	MyVector mRotation;
	MyVector mScale;

	std::vector<MyVector> mVerts;
	std::vector<std::array<size_t, 3>> mTris;
	std::vector<MyVector> mNorms;

	// Calculates initial surface normals when the object is created. After creation, mNormsTransformed
	// are calculated by the renderer class according to the object's transformation
	void calculateNormals();

};
