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
	void fakeFragmentShader(const std::vector<std::reference_wrapper<Object>>& objs);

	SDL_Renderer* mRenderer;
	int mTextureSizeX;
	int mTextureSizeY;
	SDL_Texture* mTexture;

	Camera mCamera;

	// MyMatrix mTransformationMat;
	// MyMatrix mProjectionMat;
	double mPerspectiveDist;
	MyMatrix mPerspectiveMat;

	MyMatrix mVerts;
	std::vector<unsigned char> mPixels;

};
