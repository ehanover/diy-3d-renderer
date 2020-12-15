#include "renderer.h"
#include "mymatrix.h"
#include "object.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

Renderer::Renderer(SDL_Renderer* renderer) :
	mRenderer(renderer),
	mTextureSizeX(0),
	mTextureSizeY(0),
	mTexture(),

	mCamera(),

	mTransformationMat(),
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

	mTransformationMat = MyMatrix(4, 4, perspectiveVec);
	// mProjectionMat = MyMatrix(4, 4, perspectiveVec);
}

void Renderer::render(const std::vector<Object>& objs) {
	fakeGeometryShader(objs);
	fakeFragmentShader(objs);

	 SDL_UpdateTexture
		(
		mTexture,
		NULL,
		mPixels.data(),
		mTextureSizeX * 4
		);
	SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
}

void Renderer::fakeGeometryShader(const std::vector<Object>& objs) {
	// Stores all vertex info from objs into mVerts
	std::vector<double> allVerts;
	for(size_t i=0; i<objs.size(); i++) {
		std::vector<std::array<double, 4>> oVerts = objs.at(i).verts();
		// TODO apply transformation matrix

		for(size_t v=0; v<oVerts.size(); v++) {
			for(size_t vi=0; vi<4; vi++) {
				allVerts.push_back(oVerts.at(v).at(vi));
				// std::cout << "adding elem=" << oVerts.at(v).at(vi) << std::endl;
			}
		}
		// std::cout << "done with obj" << std::endl;
	}
	mVerts = MyMatrix(allVerts.size()/4, 4, allVerts);
	// std::cout << mVerts << std::endl << std::endl;

	// Apply projection
	mVerts = mVerts.multiply(mCamera.projectionMatrix());
	// std::cout << mVerts << std::endl << std::endl;

	// Apply perspective
	mVerts = mVerts.multiply(mPerspectiveMat);
	// std::cout << mVerts << std::endl;
}

void Renderer::fakeFragmentShader(const std::vector<Object>& objs) {
	// Calculates pixel values from info in mVerts

	for(size_t i=0; i<objs.size(); i++) {
		std::vector<std::array<size_t, 3>> tris = objs.at(i).tris();

		// DEBUG ONLY
		std::vector<double> allVerts = mVerts.data();
		for(size_t dv=0; dv<mVerts.rows(); dv++) {
			double w = allVerts[dv*4 + 3];
			int x = (int) (allVerts[dv*4 + 0]/w) + 300;
			int y = (int) (allVerts[dv*4 + 1]/w) + 300;
			// std::cout << "drawing vert at x=" << vert[0] << ", y=" << vert[1] << std::endl;

			for(size_t s=0; s<8; s++) {
				mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 0] = 250;
				mPixels[(y*mTextureSizeY*4) + ((x+s)*4) + 3] = SDL_ALPHA_OPAQUE;
			}
		}
		// std::cout << "drawing done" << std::endl;
	}




	/*for(size_t i=0; i<mTextureSizeX-50; i++) {
		int row = i;
		int col = i;
		mPixels[(row*mTextureSizeX*4) + (col*4) + 0] = 200;
		mPixels[(row*mTextureSizeX*4) + (col*4) + 3] = SDL_ALPHA_OPAQUE;
	}*/
}


