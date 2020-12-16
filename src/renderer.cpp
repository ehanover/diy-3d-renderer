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

	mVerts(),
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
	// mPerspectiveMat.setData(perspectiveVec);
}

void Renderer::render(const std::vector<std::reference_wrapper<Object>>& objs) {
	clearPixels();

	fakeGeometryShader(objs);
	fakeFragmentShader(objs);

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
	for(size_t i=0; i<objs.size(); i++) {
		std::vector<double> allVerts = mVerts.data();
		for(size_t dv=0; dv<mVerts.rows(); dv++) {
			double w = allVerts[dv*4 + 3];
			int x = (int) (allVerts[dv*4 + 0]/w) + 300;
			int y = (int) (allVerts[dv*4 + 1]/w) + 300;
			// std::cout << "drawing vert at x=" << x << ", y=" << y << std::endl;

			for(int s=0; s<2; s++) {
				mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 1] = 250;
				mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 3] = SDL_ALPHA_OPAQUE;
			}
		}
	}
}

void Renderer::fakeGeometryShader(const std::vector<std::reference_wrapper<Object>>& objs) {
	// Stores all vertex info from objs into mVerts
	std::vector<double> allVerts;

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
		/*std::vector<double> rotYVec{ // (X rotation)
			1, 0, 0, 0,
			0, cos(rotY), -sin(rotY), 0,
			0, sin(rotY), cos(rotY), 0,
			0, 0, 0, 1
		};*/
		MyMatrix objVertsTransformed = objVerts.multiply(MyMatrix(4, 4, rotYVec));

		allVerts.insert(allVerts.end(), objVertsTransformed.data().begin(), objVertsTransformed.data().end());
	}
	mVerts = MyMatrix(allVerts.size()/4, 4, allVerts);

	// Apply projection
	mVerts = mVerts.multiply(mCamera.projectionMatrix());

	// Apply perspective
	mVerts = mVerts.multiply(mPerspectiveMat);
}

void Renderer::fakeFragmentShader(const std::vector<std::reference_wrapper<Object>>& objs) {
	// Calculates pixel values from info in mVerts

}
