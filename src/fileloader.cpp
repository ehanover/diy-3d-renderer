#include "fileloader.h"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unordered_set>

#define EPSILON 1.0e-8

using namespace std;

struct MyVectorHash {
public:
	// https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
	size_t operator()(const MyVector& v) const {
		size_t seed = v.size();
		for(size_t i=0; i<v.size(); i++) {
			seed ^= 0x9e3779b9 + hash<double>()(v.elem(i)) + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

struct MyVectorEqual {
public:
	bool operator()(const MyVector& v1, const MyVector& v2) const {
		if(v1.size() != v2.size()) {
			return false;
		}
		for(size_t i=0; i<v1.size(); i++) {
			if(abs(v1.elem(i) - v2.elem(i)) > EPSILON) {
				return false;
			}
		}
		return true;
	}
};


void printSet(unordered_set<MyVector, MyVectorHash, MyVectorEqual>& set) {
	unordered_set<MyVector>::iterator it;
	for(it=set.begin(); it!=set.end(); it++) {
		MyVector val = *it;
		cout << val << "| ";
	}
	cout << endl;
}

// /*
bool isBinarySTL(char* buffer) {

	// Determine if a file is binary STL.

	bool bbinary = true;
	size_t spnsz, spnsz0;

	// Look for the first non-space character.

	spnsz = strspn(buffer, " ");

	char ctstr[6];  // Enough space to hold "solid\0" and "facet\0".

	// Copy the first five characters from the location of the first non-space
	// character to ctstr.

	// strncpy_s(ctstr, &buffer[spnsz], 5);
	strncpy(ctstr, &buffer[spnsz], 5);

	ctstr[5] = '\0';
	char csolid[] = "solid\0";

	// If this is an ASCII STL file, then the first string should be "solid".

	if (!strcmp(ctstr, csolid)) {
		// This file might be binary or text. To be certain we need to do a further test.
		// Read past the next new line. If this is a text file, there should be a newline.
		// The next token should be 'facet'.

		spnsz0 = 5 + spnsz;

		char * pch = strchr(&buffer[spnsz0], '\n');  // Look for the first instance of '\n'.
		// If pch is NULL then this is a binary STL file.
		if (pch) {
			pch++;

			spnsz = strspn(pch, " "); // Look for the first instance not of ' '.
			spnsz0 = spnsz;

			spnsz = strcspn(pch + spnsz0, " "); // Look for the first instance of ' '.

			if (spnsz == 5) {
				// Check for 'facet'.
				// strncpy_s(ctstr, pch + spnsz0, 5);
				strncpy(ctstr, pch + spnsz0, 5);
				ctstr[5] = '\0';

				char cfacet[] = "facet\0";
				if (!strcmp(ctstr, cfacet)) {
					// This file is beyond reasonable doubt ASCII STL.
					bbinary = false;
				}
			}
		}
	}

	return(bbinary);
}
// */

// http://www.jgxsoft.com/examples/STL%20Reader/STL%20Reader.html
Object loadStl(const string& fileName) {

	// int nVertex = 0; // Number of vertices read.
	// int nFacet = 0;  // Number of facets read.
	unordered_set<MyVector, MyVectorHash, MyVectorEqual> verts;

	// Open the file for reading using an input fstream.
	ifstream ifs(fileName, ifstream::binary);

	// Get pointer to the associated buffer object. rdbuf returns a streambuf object associated with the input fstream object ifs.
	filebuf* pbuf = ifs.rdbuf();

	// Calculate the file's size.
	auto size = pbuf->pubseekoff(0, ifs.end);

	// Set the position pointer to the beginning of the file.
	pbuf->pubseekpos(0);

	// Allocate memory to contain file data.
	char* buffer = new char[(size_t) size];

	// Get file data. sgetn grabs all the characters from the streambuf object 'pbuf'. The return value of sgetn is the number of characters
	// obtained - ordinarily, this value should be checked for equality against the number of characters requested.
	pbuf->sgetn(buffer, size);

	// Test to see if the file is binary.
	if (!isBinarySTL(buffer)) {
		cout << "Error: STL file is not in binary format" << endl;
		return Object({}, {}, {});
	}

	char* bufptr = buffer;

	bufptr += 80; // Skip past the header.

	uint32_t numTris = *(uint32_t*) (bufptr);
	verts.reserve(numTris*3);
	bufptr += 4; // Skip past the number of triangles.
	// cout << "numTris=" << numTris << endl;

	vector<MyVector> norms;
	vector<array<MyVector, 3>> trisVec;

	while (bufptr < buffer + size) {

		float nx = *(float*) (bufptr);
		float ny = *(float*) (bufptr + 4);
		float nz = *(float*) (bufptr + 8);
		bufptr += 12;

		float v1x = *(float*) (bufptr);
		float v1y = *(float*) (bufptr + 4);
		float v1z = *(float*) (bufptr + 8);
		bufptr += 12;

		float v2x = *(float*) (bufptr);
		float v2y = *(float*) (bufptr + 4);
		float v2z = *(float*) (bufptr + 8);
		bufptr += 12;

		float v3x = *(float*) (bufptr);
		float v3y = *(float*) (bufptr + 4);
		float v3z = *(float*) (bufptr + 8);
		bufptr += 12;

		// If the normal in the STL file is blank, then create a proper normal.
		// (the calculation should be noticed and handled by object.cpp constructor)
		if (abs(nx) < EPSILON && abs(ny) < EPSILON && abs(nz) < EPSILON) {
			cout << "Warning: when importing STL, found a tri without a normal." << endl;
		}

		// nFacet++;
		// nVertex += 3;
		bufptr += 2;

		MyVector norm = MyVector({(double)nx, (double)ny, (double)nz});
		MyVector v1 = MyVector({(double)v1x, (double)v1y, (double)v1z, 1});
		MyVector v2 = MyVector({(double)v2x, (double)v2y, (double)v2z, 1});
		MyVector v3 = MyVector({(double)v3x, (double)v3y, (double)v3z, 1});
		verts.insert(v1);
		verts.insert(v2);
		verts.insert(v3);

		norms.push_back(norm);
		trisVec.push_back({v1, v2, v3});

		// cout << "v1i=" << v1i << ", v2i=" << v2i << ", v3i=" << v3i << endl;
		// cout << "norm=" << norm << endl;
	}

	vector<array<size_t, 3>> tris;
	tris.reserve(norms.size());

	for(size_t i=0; i<trisVec.size(); i++) {
		array<MyVector, 3> triVec = trisVec.at(i);
		size_t v1i = distance(verts.begin(), verts.find(triVec[0]));
		size_t v2i = distance(verts.begin(), verts.find(triVec[1]));
		size_t v3i = distance(verts.begin(), verts.find(triVec[2]));

		array<size_t, 3> tri = {v1i, v2i, v3i};
		tris.push_back(tri);

		// cout << "tri=" << tri[0] << ", " << tri[1] << ", " << tri[2] << endl;
	}

	ifs.close();
	delete[] buffer;

	vector<MyVector> vertsVec;
	vertsVec.reserve(verts.size());
	unordered_set<MyVector>::iterator it;
	for(it=verts.begin(); it!=verts.end(); it++) {
		MyVector val = *it;
		vertsVec.push_back(val);
		// cout << "pushing back val=" << val << endl;
	}

	return Object(vertsVec, tris, norms);
}

/*
// http://www.sgh1.net/posts/read-stl-file.md
Object loadStl(std::string filepath) {
	ifstream myFile (filepath.c_str(), ios::in | ios::binary); // don't forget ios::binary

	char header_info[80] = "";
	char nTri[4];
	unsigned long nTriLong;

	// read 80 byte header
	if (myFile) {
		myFile.read(header_info, 80);
		cout << "header: " << header_info << endl;
	}
	else {
		cout << "Error reading file: " << filepath << endl;
		return Object();
	}

	// read 4-byte ulong
	if(myFile) {
		myFile.read (nTri, 4);
		nTriLong = *((unsigned long*) nTri) ;
		cout <<"n Tri: " << nTriLong << endl;
	} else {
		cout << "Error reading file - parsing number of triangles" << endl;
	}

	// now read in all the triangles
	for(unsigned long i=0; i<nTriLong; i++){

		char facet[50];

		if(myFile) {
			// read one 50-byte triangle
			myFile.read (facet, 50);
			cout << "got tri=" << facet << endl;

			// populate each point of the triangle using v3::v3(char* bin);
			// facet + 12 skips the triangle's unit normal
			// v3 p1(facet+12);
			// v3 p2(facet+24);
			// v3 p3(facet+36);

			// add a new triangle to the array
			// v.push_back( tri(p1,p2,p3) );

		}
	}

	myFile.close();
	return Object();
}
*/
