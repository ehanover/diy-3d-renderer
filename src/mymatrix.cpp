#include "mymatrix.h"
#include <cstddef>
#include <iostream>
#include <vector>

MyMatrix::MyMatrix() :
	mRows(0), mCols(0), mData() {
}

MyMatrix::MyMatrix(size_t r, size_t c, std::vector<double> d) : // TODO vector d can be a reference
	mRows(r), mCols(c), mData(std::vector<double>(d)) {
}

MyMatrix::MyMatrix(const MyMatrix& o) :
	mRows(o.rows()), mCols(o.cols()), mData(std::vector<double>(o.data())) {
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

const double& MyMatrix::elem(size_t i) const {
	return mData.at(i);
}

const double& MyMatrix::elem(size_t r, size_t c) const {
	return elem(r * cols() + c);
}

MyMatrix& MyMatrix::add(const MyMatrix& o) {
	if(o.rows() != rows() || o.cols() != cols()) {
		std::cerr << "Error: tried adding matrices with wrong sizes, skipping" << std::endl;
		return *this;
	}
	std::vector<double> ret(rows() * cols());
	for(size_t i=0; i<rows() * cols(); i++) {
		mData[i] += o.elem(i);
	}
	return *this;
}

MyMatrix& MyMatrix::multiply(double o) {
	for(size_t i=0; i<rows() * cols(); i++) {
		mData[i] *= o;
	}
	return *this;
}

MyMatrix& MyMatrix::multiply(const MyMatrix& o) {
	// https://www.baeldung.com/cs/MyMatrix-multiplication-algorithms
	if(cols() != o.rows()) {
		std::cerr << "Error: tried multiplying matrices with wrong sizes, skipping" << std::endl;
	}
	size_t newRows = rows();
	size_t newCols = o.cols();
	std::vector<double> d(newRows * newCols);
	for(size_t i=0; i<newRows; i++) {
		for(size_t j=0; j<newCols; j++) {
			d[i*newCols + j] = 0;
			for(size_t k=0; k<o.rows(); k++) {
				d[i*newCols + j] += (elem(i, k) * o.elem(k, j));
			}
		}
	}
	mData = d;
	mRows = newRows;
	mCols = newCols;
	return *this;
}

std::ostream& operator<<(std::ostream& out, const MyMatrix& m) {
	out.precision(3);
	for(size_t i=0; i<m.rows(); i++) {
		for(size_t j=0; j<m.cols(); j++) {
			out << std::fixed << m.elem(i, j) << " ";
		}
		if(i < m.rows()-1) {
			out << std::endl;
		}
	}
	return out;
}
