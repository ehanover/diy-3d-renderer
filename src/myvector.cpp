#include "myvector.h"
#include <cmath>
#include <cstddef>
#include <vector>

MyVector::MyVector(size_t s, std::vector<double> d) {
	mSize = s;
	mData = new std::vector<double>(d);
	// for(size_t i=0; i<s; i++) {
		// mData->at(i) = d[i];
		// std::cout << "just set val " << mData->at(i) << std::endl;
	// }
	// std::cout << "cons main" << std::endl;
}

MyVector::MyVector(const MyVector& o) {
	mSize = o.size();
	mData = new std::vector<double>(o.data());
	// std::cout << "copy constructor msize=" << mSize << std::endl;
	// std::cout << "just set msize " << mSize << std::endl;
	// for(size_t i=0; i<mSize; i++) {
	// 	mData->at(i) = o.elem(i);
		// std::cout << "just set val " << mData->at(i) << std::endl;
	// }
	// std::cout << "cons copy" << std::endl;
}

size_t MyVector::size() const {
    return mSize;
}

MyVector::~MyVector() {
	delete mData;
}

const std::vector<double>& MyVector::data() const {
	return *mData;
}

const double& MyVector::elem(size_t i) const {
	return mData->at(i);
}

double MyVector::magnitude() const {
	double sum = 0;
	for(size_t i=0; i<size(); i++) {
		sum += pow(elem(i), 2);
	}
	return sqrt(sum);
}

MyVector MyVector::add(const MyVector& rhs) {
	if(rhs.size() != size()) {
		std::cerr << "Error: tried adding vectors of different sizes, skipping" << std::endl;
		return *this;
	}
	std::vector<double> ret(data());
	for(size_t i=0; i<size(); i++) {
		ret[i] += rhs.elem(i);
	}
	return MyVector(size(), ret);
}

MyVector MyVector::scalar(const double& rhs) {
	std::vector<double> ret(data());
	for(size_t i=0; i<size(); i++) {
		ret[i] *= rhs;
	}
	return MyVector(size(), ret);
}

double MyVector::dot(const MyVector& rhs) {
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

MyVector MyVector::cross(const MyVector& rhs) {
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
	// mData[0] = nx;
	// mData[1] = ny;
	// mData[2] = nz;
	// mData->at(0) = nx;
	// mData->at(1) = ny;
	// mData->at(2) = nz;
	return MyVector(size(), std::vector<double>{nx, ny, nz});
}

MyVector MyVector::normalize() {
	double m = magnitude();
	std::vector<double> ret(data());
	for(size_t i=0; i<size(); i++) {
		ret[i] /= m;
	}
	return MyVector(size(), ret);
}

std::ostream& operator<<(std::ostream& out, const MyVector& MyVector) {
	out.precision(3);
	for(size_t i=0; i<MyVector.size(); i++) {
		out << MyVector.elem(i) << " ";
	}
	return out;
}
