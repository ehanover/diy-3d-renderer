#include "renderer.h"
#include "mymatrix.h"
#include "object.h"
#include <SDL2/SDL.h>
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

	mObjs(),
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
	// clearPixels(); // mPixels should probably be instantiated new every loop
	mObjs.clear();

	fakeGeometryShader(objs);
	fakeFragmentShader();

	drawDebugVerts(objs);

	 SDL_UpdateTexture
		(
		mTexture,
		NULL,
		mPixels.data(),
		mTextureSizeX * 4
		);
	SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
}

void Renderer::clearPixels() {
	for(int i=0; i<(mTextureSizeX * mTextureSizeY * 4); i++) {
		mPixels[i] = 0;
	}
}

void Renderer::drawDebugVerts(const std::vector<std::reference_wrapper<Object>>& objs) {
	for(size_t i=0; i<mObjs.size(); i++) {
		Object obj = mObjs.at(i);
		MyMatrix objMat = obj.verts();
		for(size_t r=0; r<objMat.rows(); r++) {
			std::array<double, 2> px = getVertFromMat(objMat, r);
			int x = (int) px[0];
			int y = (int) px[1];
			// std::cout << "drawing vert at x=" << x << ", y=" << y << std::endl;

			for(int s=0; s<3; s++) {
				mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 1] = 250;
				mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 3] = SDL_ALPHA_OPAQUE;
			}
		}
	}
}

void Renderer::fakeGeometryShader(const std::vector<std::reference_wrapper<Object>>& objs) {
	// Stores all vertex info from objs into mVerts

	// Objects should probably have a matrix for verts instead of a vector
	for(size_t i=0; i<objs.size(); i++) {
		Object obj = objs.at(i).get();
		MyMatrix objVerts = obj.verts();

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
		/*std::vector<double> rotYVec{ // (X rotation)
			1, 0, 0, 0,
			0, cos(rotY), -sin(rotY), 0,
			0, sin(rotY), cos(rotY), 0,
			0, 0, 0, 1
		};*/

		objVerts.multiply(transMat);
		objVerts.multiply(mCamera.projectionMatrix());
		objVerts.multiply(mPerspectiveMat);
		obj.setVerts(objVerts);
		mObjs.push_back(obj);
	}

}

std::array<double, 2> Renderer::getVertFromMat(const MyMatrix& m, size_t row) {
	double w = m.elem(row, 3);
	return {(m.elem(row, 0)/w) + (mTextureSizeX/2), (m.elem(row, 1)/w) + (mTextureSizeY/2)};
}

double Renderer::edgeFunction(const std::array<double, 2>& a, const std::array<double, 2>& b, const std::array<double, 2>& c) {
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
	// return (a[0] - b[0]) * (c[1] - a[1]) - (a[1] - b[1]) * (c[0] - a[0]); // Supposed to fix rasterizing if vertices declared clockwise
}

void Renderer::fakeFragmentShader() {
	// Calculates pixel values from info in mObjs

	// Rasterize
	for(int y=0; y<mTextureSizeY; y++) {
		for(int x=0; x<mTextureSizeX; x++) {

			int arrayOffset = (y*mTextureSizeX*4) + (x*4);
			mPixels[arrayOffset + 0] = 0;
			mPixels[arrayOffset + 1] = 0;
			mPixels[arrayOffset + 2] = 0;
			mPixels[arrayOffset + 3] = SDL_ALPHA_OPAQUE;

			int originOffsetX = (mTextureSizeX/2);
			int originOffsetY = (mTextureSizeY/2);
			std::array<double, 2> pixVert = {(double)x, (double)y};
			for(size_t oi=0; oi<mObjs.size(); oi++) {
				Object& obj = mObjs.at(oi); // Could this be a reference?
				MyMatrix objMat = obj.verts();
				std::vector<std::array<size_t, 3>> objTris = obj.tris();
				for(size_t ti=0; ti<objTris.size(); ti++) {
					// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
					std::array<size_t, 3> tri = objTris.at(ti);
					std::array<double, 2> triVert1 = getVertFromMat(objMat, tri[0]);
					std::array<double, 2> triVert2 = getVertFromMat(objMat, tri[1]);
					std::array<double, 2> triVert3 = getVertFromMat(objMat, tri[2]);

					double w1 = -edgeFunction(triVert2, triVert3, pixVert); // These aren't supposed to be multiplied by -1
					double w2 = -edgeFunction(triVert3, triVert1, pixVert);
					double w3 = -edgeFunction(triVert1, triVert2, pixVert);
					if(w1 >= 0 && w2 >= 0 && w3 >= 0) {
						mPixels[arrayOffset + 0] = 250;
						mPixels[arrayOffset + 3] = SDL_ALPHA_OPAQUE;
					}

				}
			}
		}
	}
	

}
