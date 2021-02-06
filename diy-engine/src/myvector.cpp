#include "myvector.h"
#include <cmath>

MyVector::MyVector(std::vector<double> d) :
	mSize(d.size()), mData(std::vector<double>(d)) {

}

MyVector::MyVector(const MyVector& o) :
	mSize(o.size()), mData(std::vector<double>(o.data())) {

}

size_t MyVector::size() const {
	return mSize;
}

const std::vector<double>& MyVector::data() const {
	return mData;
}

// const double& MyVector::elem(size_t i) const { // (moved to inline)
// 	return mData.at(i);
// }

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
	// I could to the Quake fast inverse square root trick here, but I tested it and found that it's somehow slower
	double m = magnitude();
	return scalar(1/m);
}

MyVector& MyVector::multiplyByMatrix(const MyMatrix& rhs) {
	// https://www.scratchapixel.com/code.php?id=22&origin=/lessons/mathematics-physics-for-computer-graphics/geometry
	
	if(rhs.rows() == rhs.cols() && size() == 3) {
		// For vectors, 3 components
		double a = elem(0)*rhs.elem(0,0) + elem(1)*rhs.elem(1,0) + elem(2)*rhs.elem(2,0); // + rhs.elem(3,0);
		double b = elem(0)*rhs.elem(0,1) + elem(1)*rhs.elem(1,1) + elem(2)*rhs.elem(2,1); // + rhs.elem(3,1);
		double c = elem(0)*rhs.elem(0,2) + elem(1)*rhs.elem(1,2) + elem(2)*rhs.elem(2,2); // + rhs.elem(3,2);
		mData[0] = a;
		mData[1] = b;
		mData[2] = c;
	} else if (rhs.rows() == 4 && rhs.cols() == 4 && size() == 4) {
		// For points, 3 coordinates + w component = 4 components
		double a = elem(0)*rhs.elem(0,0) + elem(1)*rhs.elem(1,0) + elem(2)*rhs.elem(2,0) + elem(3)*rhs.elem(3,0);
		double b = elem(0)*rhs.elem(0,1) + elem(1)*rhs.elem(1,1) + elem(2)*rhs.elem(2,1) + elem(3)*rhs.elem(3,1);
		double c = elem(0)*rhs.elem(0,2) + elem(1)*rhs.elem(1,2) + elem(2)*rhs.elem(2,2) + elem(3)*rhs.elem(3,2);
		double w = elem(0)*rhs.elem(0,3) + elem(1)*rhs.elem(1,3) + elem(2)*rhs.elem(2,3) + elem(3)*rhs.elem(3,3);
		mData[0] = a;
		mData[1] = b;
		mData[2] = c;
		mData[3] = w;
	} else {
		std::cout << "Error: tried multiplying a vector with a matrix of wrong size, skipping" << std::endl;
	}
	return *this;
}

MyVector& MyVector::dropW() {
	if(mSize != 4) {
		std::cout << "Error: tried dropping w component on a vector of size != 3, skipping" << std::endl;
		return *this;
	}
	if(mData[3] > 1.001 || mData[3] < 0.999) {
		std::cout << "Warning: tried dropping w component on a vector with w != 1, dropping anyway (w=" << mData[3] << ")" << std::endl;
	}
	mSize = 3;
	mData.erase(mData.begin() + 3);
	return *this;
}

std::ostream& operator<<(std::ostream& out, const MyVector& v) {
	out.precision(3);
	for(size_t i=0; i<v.size(); i++) {
		out << v.elem(i) << " ";
	}
	return out;
}
