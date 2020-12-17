#include "myvector.h"
#include <cmath>
#include <cstddef>
#include <vector>

MyVector::MyVector(std::vector<double> d) :
	mSize(d.size()), mData(std::vector<double>(d)) {
	// std::cout << "cons main" << std::endl;
}

MyVector::MyVector(const MyVector& o) :
	mSize(o.size()), mData(std::vector<double>(o.data())) {
	// std::cout << "cons copy" << std::endl;
}

size_t MyVector::size() const {
	return mSize;
}

const std::vector<double>& MyVector::data() const {
	return mData;
}

const double& MyVector::elem(size_t i) const {
	return mData.at(i);
}

double MyVector::magnitude() const {
	double sum = 0;
	for(size_t i=0; i<size(); i++) {
		sum += pow(elem(i), 2);
	}
	return sqrt(sum);
}

double MyVector::dot(const MyVector& rhs) const {
	if(rhs.size() != size()) {
		std::cerr << "Error: tried dotting vectors of different sizes, returning 0" << std::endl;
		return 0;
	}
	double sum = 0;
	const std::vector<double>& lhsData = data();
	const std::vector<double>& rhsData = rhs.data();
	for(size_t i=0; i<size(); i++) {
		sum += lhsData[i] * rhsData[i];
	}
	return sum;
}

MyVector& MyVector::add(const MyVector& rhs) {
	if(rhs.size() != size()) {
		std::cerr << "Error: tried adding vectors of different sizes, skipping" << std::endl;
		return *this;
	}
	for(size_t i=0; i<size(); i++) {
		mData[i] += rhs.elem(i);
	}
	return *this;
}

MyVector& MyVector::scalar(const double& rhs) {
	for(size_t i=0; i<size(); i++) {
		mData[i] *= rhs;
	}
	return *this;
}



MyVector& MyVector::cross(const MyVector& rhs) {
	if(rhs.size() != size()) {
		std::cerr << "Error: tried crossing vectors of different sizes, skipping" << std::endl;
		return *this;
	}
	if(size() != 3) {
		std::cerr << "Error: tried crossing vectors with size != 3, skipping" << std::endl;
		return *this;
	}
	double nx = elem(1)*rhs.elem(2) - elem(2)*rhs.elem(1);
	double ny = elem(2)*rhs.elem(0) - elem(0)*rhs.elem(2);
	double nz = elem(0)*rhs.elem(1) - elem(1)*rhs.elem(0);
	mData[0] = nx;
	mData[1] = ny;
	mData[2] = nz;
	return *this;
}

MyVector& MyVector::normalize() {
	double m = magnitude();
	return scalar(1/m);
}

std::ostream& operator<<(std::ostream& out, const MyVector& v) {
	out.precision(3);
	for(size_t i=0; i<v.size(); i++) {
		out << v.elem(i) << " ";
	}
	return out;
}
