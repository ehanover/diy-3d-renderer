#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

class MyVector {

public:

	MyVector(size_t s, std::vector<double> d);
	MyVector(const MyVector& o);

	const double& elem(size_t i) const;

	size_t size() const;
	const std::vector<double>& data() const;
	double magnitude() const;

	MyVector& add(const MyVector& rhs);
	MyVector& scalar(const double& rhs);
	double dot(const MyVector& rhs);
	MyVector& cross(const MyVector& rhs);
	MyVector& normalize();

	void operator+=(const MyVector& rhs);
	void operator*=(const double& rhs);

private:
	size_t mSize;
	std::vector<double> mData;

};

std::ostream& operator<<(std::ostream& out, const MyVector& MyVector);
MyVector operator+(const MyVector& lhs, const MyVector& rhs);
MyVector operator*(const double scalar, const MyVector& rhs);
