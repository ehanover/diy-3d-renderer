#pragma once

#include "mymatrix.h"
#include <cstddef>
#include <iostream>
#include <vector>

class MyVector {

public:

	MyVector(std::vector<double> d);
	MyVector(const MyVector& o);

	const double& elem(size_t i) const;

	size_t size() const;
	const std::vector<double>& data() const;
	double magnitude() const;
	double dot(const MyVector& rhs) const;

	MyVector& add(const MyVector& rhs);
	MyVector& scalar(const double& rhs);
	MyVector& cross(const MyVector& rhs);
	MyVector& normalize();
	MyVector& multiplyByMatrix(const MyMatrix& rhs);
	MyVector& dropW();

private:
	size_t mSize;
	// std::vector<double>* mData;
	std::vector<double> mData;

};

std::ostream& operator<<(std::ostream& out, const MyVector& v);
