#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

// http://codeofthedamned.com/index.php/introduction-to-the-math-of
class MyMatrix {

public:
	// Ctor and Dtor omitted (?)
	MyMatrix(size_t r, size_t c, std::vector<double> d);
	MyMatrix(const MyMatrix& o);

	// Calculate and return a reference to the specified element.
	const double& elem(size_t row, size_t column) const;
	const double& elem(size_t i) const;

	// Resizes this matrix to have the specified size.
	// void resize(size_t row, size_t column);
	
	size_t rows() const;
	size_t cols() const;
	const std::vector<double>& data() const;

	void operator+=(const MyMatrix& rhs);
	void operator*=(const double& rhs);

private:
	size_t mRows;
	size_t mCols;
	std::vector<double> mData;

};

std::ostream& operator<<(std::ostream& out, const MyMatrix& MyMatrix);
MyMatrix operator+(const MyMatrix& lhs, const MyMatrix& rhs);
MyMatrix operator*(const double scalar, const MyMatrix& rhs);
MyMatrix operator*(const MyMatrix& lhs, const MyMatrix& rhs);
