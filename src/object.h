#pragma once

#include "mymatrix.h"
#include "myvector.h"
#include <array>
#include <cstddef>
#include <vector>

class Object {

public:
	Object(MyMatrix vs, std::vector<std::array<size_t, 3>> ts);
	Object(const Object& o);

	MyMatrix verts() const;
	std::vector<std::array<size_t, 3>> tris() const;

	MyVector position() const;
	MyVector rotation() const;
	MyVector scale() const;

	void setVerts(MyMatrix v);
	void setPosition(MyVector p);
	void setRotation(MyVector r);
	void setScale(MyVector s);

private:
	MyMatrix mVerts;
	std::vector<std::array<size_t, 3>> mTris;

	MyVector mPosition;
	MyVector mRotation;
	MyVector mScale;

};