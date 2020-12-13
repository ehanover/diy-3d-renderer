#include "mymatrix.h"
#include <cstddef>
#include <iostream>
#include <vector>

MyMatrix::MyMatrix(size_t r, size_t c, std::vector<double> d) : 
		mRows(r), mCols(c), mData(d) {
}

MyMatrix::MyMatrix(const MyMatrix& o) : 
	mRows(o.rows()), mCols(o.cols()), mData(o.data()) {
}

size_t MyMatrix::rows() const {
	return mRows;
}

size_t MyMatrix::cols() const {
	return mCols;
}

const std::vector<double>& MyMatrix::data() const {
	return mData;
}

const double& MyMatrix::elem(size_t r, size_t c) const {
	return mData[r * cols() + c];
}

const double& MyMatrix::elem(size_t i) const {
	return mData[i];
}

// Operators

void MyMatrix::operator+=(const MyMatrix& rhs) {
	if(rhs.rows() != rows() || rhs.cols() != cols()) {
		std::cerr << "Error: tried adding matrices of different sizes, skipping" << std::endl;
		return;
	}
	for(size_t i=0; i<mData.size(); i++) {
		mData[i] += rhs.elem(i);
	}
}

void MyMatrix::operator*=(const double& rhs) {
	for(size_t i=0; i<mData.size(); i++) {
		mData[i] *= rhs;
	}
}

std::ostream& operator<<(std::ostream& out, const MyMatrix& MyMatrix) {
	out.precision(3);
	for(size_t i=0; i<MyMatrix.rows(); i++) {
		for(size_t j=0; j<MyMatrix.cols(); j++) {
			out << std::fixed << MyMatrix.elem(i, j) << " ";
		}
		if(i < MyMatrix.rows()-1) {
			out << std::endl;
		}
	}
	return out;
}

MyMatrix operator+(const MyMatrix& lhs, const MyMatrix& rhs) {
	MyMatrix r(lhs);
	r += rhs;
	return r;
}

MyMatrix operator*(const double scalar, const MyMatrix& rhs) {
	MyMatrix r(rhs);
	r *= scalar;
	return r;
}

MyMatrix operator*(const MyMatrix& lhs, const MyMatrix& rhs) {
	// https://www.baeldung.com/cs/MyMatrix-multiplication-algorithms
	if(lhs.cols() != rhs.rows()) {
		std::cerr << "Error: tried multiplying matrices with wrong sizes, skipping" << std::endl;
		return lhs;
	}
	size_t newRows = lhs.rows();
	size_t newCols = rhs.cols();
	std::vector<double> r(newRows * newCols);
	for(size_t i=0; i<lhs.rows(); i++) {
		for(size_t j=0; j<rhs.cols(); j++) {
			r[i*newCols + j] = 0;
			for(size_t k=0; k<rhs.rows(); k++) {
				r[i*newCols + j] = r[i*newCols + j] + (lhs.elem(i, k) * rhs.elem(k, j));
			}
		}
	}
	return MyMatrix(newRows, newCols, r);
}
