#include "renderer.h"
#include "mymatrix.h"
#include "object.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

Renderer::Renderer(SDL_Renderer* renderer) :
	mRenderer(renderer),
	mTextureSizeX(0),
	mTextureSizeY(0),
	mTexture(),

	mCamera(),

	// mTransformationMat(),
	// mProjectionMat(),
	mPerspectiveDist(1.0),
	mPerspectiveMat(),

	mVertsRender(),
	mTrisRender(),
	mPixels()
{
	SDL_GetRendererOutputSize(mRenderer, &mTextureSizeX, &mTextureSizeY);
	mTexture = SDL_CreateTexture
		(
		mRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		mTextureSizeX, mTextureSizeY // What is the significance of the texture dimension?
		);
	mPixels = std::vector<unsigned char>(mTextureSizeX * mTextureSizeY * 4);

	// Perspective
	std::vector<double> perspectiveVec{
		(double)mTextureSizeX, 0, 0, 0,
		0, (double)mTextureSizeY, 0, 0,
		0, 0, 1, (1/mPerspectiveDist),
		0, 0, 0, 0
	};
	mPerspectiveMat = MyMatrix(4, 4, perspectiveVec);
}

void Renderer::render(const std::vector<std::reference_wrapper<Object>>& objs) {
	std::fill(mPixels.begin(), mPixels.end(), 0);
	mVertsRender.clear();
	mTrisRender.clear();
	mNormsRender.clear();
	// mColorsRender.clear();

	fakeVertexShader(objs); // The vertex shader takes in objs and breaks them down to verts+tris+norms+etc stored in member vars
	fakeGeometryShader();
	fakeFragmentShader();

	drawDebugVerts();

	 SDL_UpdateTexture
		(
		mTexture,
		NULL,
		mPixels.data(),
		mTextureSizeX * 4
		);
	SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
}

void Renderer::drawDebugVerts() {
	for(size_t i=0; i<mVertsRender.size(); i++) {
		// Assuming these vectors' w component is 1
		MyVector vert = mVertsRender.at(i);
		int x = (int) vert.elem(0) + (mTextureSizeX/2);
		int y = (int) vert.elem(1) + (mTextureSizeY/2);
		// std::cout << "drawing vert at x=" << x << ", y=" << y << std::endl;

		for(int s=0; s<3; s++) {
			mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 1] = 250;
			mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 3] = SDL_ALPHA_OPAQUE;
		}
	}
}

void Renderer::fakeVertexShader(const std::vector<std::reference_wrapper<Object>>& objs) {
	for(size_t i=0; i<objs.size(); i++) {
		Object obj = objs.at(i).get();
		std::vector<MyVector> objVerts = obj.verts();
		std::vector<std::array<size_t, 3>> objTris = obj.tris();
		std::vector<MyVector> objNorms = obj.norms();

		// Apply transformation matrix
		// Y rotation
		double rotY = obj.rotation().elem(1);
		std::vector<double> rotYVec{
			cos(rotY), 0, -sin(rotY), 0,
			0, 1, 0, 0,
			sin(rotY), 0, cos(rotY), 0,
			0, 0, 0, 1
		};
		MyMatrix transMat = MyMatrix(4, 4, rotYVec);
		// std::vector<double> rotYVec{ // (X rotation)
		// 	1, 0, 0, 0,
		// 	0, cos(rotY), -sin(rotY), 0,
		// 	0, sin(rotY), cos(rotY), 0,
		// 	0, 0, 0, 1
		// };
		// Multiply all transformations together before looping over object verts

		for(size_t j=0; j<objVerts.size(); j++) {
			MyVector objVert = objVerts.at(j);

			objVert.multiplyByMatrix(transMat);
			objVert.multiplyByMatrix(mCamera.projectionMatrix()); // here
			objVert.multiplyByMatrix(mPerspectiveMat);
			objVert.scalar(1.0/objVert.elem(3)); // Homogenize

			mVertsRender.push_back(objVert);
		}

		MyMatrix transMatNormals(transMat); // Used to transform the normal vectors along with the vert transformations
		transMatNormals.invert();
		transMatNormals.transpose();

		for(size_t j=0; j<objTris.size(); j++) {
			mTrisRender.push_back(objTris.at(j));

			// https://www.scratchapixel.com/lessons/3d-basic-rendering/transforming-objects-using-matrices
			// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
			MyVector objNorm = objNorms.at(j); // Multiply normals by the tanspose of the inverse of the object-to-camera matrix
			objNorm.multiplyByMatrix(transMatNormals);
			mNormsRender.push_back(objNorm);
		}
	}

}

void Renderer::fakeGeometryShader() {
	// Nothing will ever happen here, probably
}

std::array<double, 2> Renderer::getCoordFromVert(const MyVector& v) {
	// double w = m.elem(row, 3);
	double w = 1; // TODO is this assumption always safe?
	return {(v.elem(0)/w) + (mTextureSizeX/2), (v.elem(1)/w) + (mTextureSizeY/2)};
}

double Renderer::edgeFunction(const std::array<double, 2>& a, const std::array<double, 2>& b, const std::array<double, 2>& c) {
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
	// return (a[0] - b[0]) * (c[1] - a[1]) - (a[1] - b[1]) * (c[0] - a[0]); // Supposed to fix rasterizing if vertices declared clockwise
}

double Renderer::facingRatio(const MyVector& norm) {
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-normals
	return norm.dot(mCamera.cameraEye());
}

void Renderer::fakeFragmentShader() {
	// Calculates pixel values from verts+tris+norms by rasterizing
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
	
	for(size_t i=0; i<mTrisRender.size(); i++) {

		// Assuming mTrisRender and mNormsRender have the same size
		std::array<size_t, 3>& tri = mTrisRender.at(i);
		MyVector& norm = mNormsRender.at(i);
		double ratio = facingRatio(norm);
		if(ratio >= 0) {
			continue;
		}
		// Assuming these vectors' w component is 1
		std::array<double, 2> triVert1 = getCoordFromVert(mVertsRender.at(tri[0]));
		std::array<double, 2> triVert2 = getCoordFromVert(mVertsRender.at(tri[1]));
		std::array<double, 2> triVert3 = getCoordFromVert(mVertsRender.at(tri[2]));

		std::array<double, 3> triXs = {triVert1[0], triVert2[0], triVert3[0]};
		std::array<double, 3> triYs = {triVert1[1], triVert2[1], triVert3[1]};
		std::pair<double*, double*> triXsBounds = std::minmax_element(triXs.begin(), triXs.end());
		std::pair<double*, double*> triYsBounds = std::minmax_element(triYs.begin(), triYs.end());
		for(int y=(int)*(triYsBounds.first); y<(int)*(triYsBounds.second); y++) {
			for(int x=(int)*(triXsBounds.first); x<(int)*(triXsBounds.second); x++) {

				std::array<double, 2> pixVert = {(double)x, (double)y};

				double w1 = -edgeFunction(triVert2, triVert3, pixVert); // These aren't supposed to be multiplied by -1
				double w2 = -edgeFunction(triVert3, triVert1, pixVert);
				double w3 = -edgeFunction(triVert1, triVert2, pixVert);
				
				int arrayOffset = (y*mTextureSizeX*4) + (x*4);
				if(w1 >= 0 && w2 >= 0 && w3 >= 0) {
					mPixels[arrayOffset + 0] = 250;
					mPixels[arrayOffset + 3] = SDL_ALPHA_OPAQUE;
				}

			}
		}
	}

}
