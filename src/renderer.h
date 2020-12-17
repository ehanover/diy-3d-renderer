#pragma once

#include "camera.h"
#include "mymatrix.h"
#include "object.h"
#include <SDL2/SDL.h>
#include <functional>
#include <vector>

class Renderer {

public:
	Renderer(SDL_Renderer* renderer);

	void render(const std::vector<std::reference_wrapper<Object>>& objs);

private:

	void clearPixels();
	void drawDebugVerts(const std::vector<std::reference_wrapper<Object>>& objs);

	void fakeGeometryShader(const std::vector<std::reference_wrapper<Object>>& objs);

	std::array<double, 2> getVertFromMat(const MyMatrix& m, size_t row);
	double edgeFunction(const std::array<double, 2>& a, const std::array<double, 2>& b, const std::array<double, 2>& c);
	void fakeFragmentShader();

	SDL_Renderer* mRenderer;
	int mTextureSizeX;
	int mTextureSizeY;
	SDL_Texture* mTexture;

	Camera mCamera;

	// MyMatrix mTransformationMat;
	// MyMatrix mProjectionMat;
	double mPerspectiveDist;
	MyMatrix mPerspectiveMat;

	// MyMatrix mVerts;
	std::vector<Object> mObjs;
	std::vector<unsigned char> mPixels;

};
