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

MyMatrix& MyMatrix::invert() {
	// https://www.scratchapixel.com/code.php?id=22&origin=/lessons/mathematics-physics-for-computer-graphics/geometry

	// To test:
	// https://www.symbolab.com/solver/matrix-inverse-calculator/inverse%20%5Cbegin%7Bpmatrix%7D0%262%268%266%5C%5C%203%267%261%260%5C%5C%200%260%261%262%5C%5C%200%261%260%261%5Cend%7Bpmatrix%7D
	// MyMatrix m = MyMatrix(4, 4, std::vector<double>{0,2,8,6, 3,7,1,0, 0,0,1,2, 0,1,0,1});
	// cout << "m=" << endl << m << endl;
	// m.invert();
	// cout << "m inverse=" << endl << m << endl;

	int i, j, k;
	std::vector<double> s{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}; // Identity matrix
	std::vector<double> t(mData);

	// Forward elimination
	for (i = 0; i < 3 ; i++) {
		int pivot = i;
		double pivotsize = t[i*4 + i];

		if (pivotsize < 0)
			pivotsize = -pivotsize;

		for (j = i + 1; j < 4; j++) {
			double tmp = t[j*4 + i];

			if (tmp < 0)
				tmp = -tmp;

			if (tmp > pivotsize) {
				pivot = j;
				pivotsize = tmp;
			}
		}
		if (pivotsize == 0) {
			// Cannot invert singular matrix
			std::cout << "Error: cannot invert singular matrix, skipping" << std::endl;
			return *this;
		}
		if (pivot != i) {
			for (j = 0; j < 4; j++) {
				double tmp;

				tmp = t[i*4 + j];
				t[i*4 + j] = t[pivot*4 + j];
				t[pivot*4 + j] = tmp;

				tmp = s[i*4 + j];
				s[i*4 + j] = s[pivot*4 + j];
				s[pivot*4 + j] = tmp;
			}
		}
		for (j = i + 1; j < 4; j++) {
			double f = t[j*4 + i] / t[i*4 + i];

			for (k = 0; k < 4; k++) {
				t[j*4 + k] -= f * t[i*4 + k];
				s[j*4 + k] -= f * s[i*4 + k];
			}
		}
	}

	// Backward substitution
	for (i = 3; i >= 0; --i) {
		double f;

		if ((f = t[i*4 + i]) == 0) {
			// Cannot invert singular matrix
			std::cout << "Error: cannot invert singular matrix, skipping" << std::endl;
			return *this;
		}
		for (j = 0; j < 4; j++) {
			t[i*4 + j] /= f;
			s[i*4 + j] /= f;
		}
		for (j = 0; j < i; j++) {
			f = t[j*4 + i];

			for (k = 0; k < 4; k++) {
				t[j*4 + k] -= f * t[i*4 + k];
				s[j*4 + k] -= f * s[i*4 + k];
			} 
		} 
	} 
	mData = s;
	return *this;
}

MyMatrix& MyMatrix::transpose() {
	if(rows() != cols()) {
		std::cout << "Error: trying to transpose a matrix that isn't square, skipping" << std::endl;
	}
	std::vector<double> d(rows()*cols());
	for(size_t i=0; i<rows(); i++) {
		for(size_t j=0; j<cols(); j++) {
			d[i*4 + j] = mData[j*4 + i];
		}
	}
	mData = d;
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
