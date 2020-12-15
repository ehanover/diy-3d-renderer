#pragma once

#include "camera.h"
#include "mymatrix.h"
#include "object.h"
#include <SDL2/SDL.h>
#include <vector>

class Renderer {

public:
	Renderer(SDL_Renderer* renderer);

	void render(const std::vector<Object>& objs);

private:

	void fakeGeometryShader(const std::vector<Object>& objs);
	void fakeFragmentShader(const std::vector<Object>& objs);

	void moveCamera(MyVector newEye, MyVector newAt, MyVector newUp);

	SDL_Renderer* mRenderer;
	int mTextureSizeX;
	int mTextureSizeY;
	SDL_Texture* mTexture;

	Camera mCamera;

	MyMatrix mTransformationMat;
	// MyMatrix mProjectionMat;
	double mPerspectiveDist;
	MyMatrix mPerspectiveMat;

	MyMatrix mVerts;
	std::vector<unsigned char> mPixels;

};