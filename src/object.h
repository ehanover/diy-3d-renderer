#pragma once

#include "myvector.h"
#include <array>
#include <cstddef>
#include <vector>

class Object {

public:
	Object(std::vector<std::array<double, 4>> vs, std::vector<std::array<size_t, 3>> ts);

	std::vector<std::array<double, 4>> verts() const;
	std::vector<std::array<size_t, 3>> tris() const;

	MyVector position() const;
	MyVector scale() const;

	void setPosition(MyVector d);
	void setScale(MyVector d);

private:
	std::vector<std::array<double, 4>> mVerts;
	std::vector<std::array<size_t, 3>> mTris;

	MyVector mPosition;
	MyVector mScale;

};