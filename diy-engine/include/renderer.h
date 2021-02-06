#pragma once

#include "camera.h"
#include "light.h"
#include "myvector.h"
#include "mymatrix.h"
#include "object.h"
#include <SDL2/SDL.h>
#include <array>
#include <functional>
#include <vector>

class Renderer {

public:
	Renderer(SDL_Renderer* renderer, float textureScale, Camera camera, double zNear, double zFar);

	void render(const std::vector<std::reference_wrapper<Object>>& objs, const Light& light);

private:

	void drawDebugVerts();

	/*
	Vertex shader:
	A vertex shader receives a single vertex from the vertex stream and generates a single vertex to the output vertex stream.

	-Transform the vert position into the screen position
	-Use the normal vector (either face or vertex) to calculate a color for each vert
		-If using face normals, the colors will be the same for each vert in a tri
	*/
	void fakeVertexShader(const std::vector<std::reference_wrapper<Object>>& objs);

	/*
	Geometry shader:
	Between the vertex and the fragment shader, there is an optional shader stage called the geometry shader. 
	A geometry shader takes as input a set of vertices that form a single primitive e.g. a point or a triangle. 
	The geometry shader can then transform these vertices as it sees fit before sending them to the next shader stage. 
	What makes the geometry shader interesting is that it is able to convert the original primitive (set of vertices) to 
	completely different primitives, possibly generating more vertices than were initially given. 
	*/
	void fakeGeometryShader();

	/*
	Fragment shader:
	Generates a color for every pixel, using steps like rasterization.
	*/
	void fakeFragmentShader(const Light& light);
	std::vector<double> shiftVertOrigin(const MyVector& v);
	double edgeFunction(const MyVector& a, const MyVector& b, const MyVector& c);
	uint16_t scaledVertDepth(double dw);

	SDL_Renderer* mRenderer;
	int mTextureSizeX;
	int mTextureSizeY;
	SDL_Rect mTextureScaledBounds;
	SDL_Texture* mTexture;

	Camera mCamera;
	double mZNear;
	double mZFar;
	MyMatrix mPerspectiveMat;

	std::vector<MyVector> mVertsWorldRender;
	std::vector<MyVector> mVertsScreenRender;
	std::vector<std::array<uint8_t, 3>> mColorsRender; // (R, G, B) The alpha component is assumed to be 255 always
	std::vector<std::array<size_t, 3>> mTrisRender;
	std::vector<MyVector> mNormsRender;

	std::vector<uint8_t> mPixels;
	std::vector<uint16_t> mDepths; // uint16_t is lowest level of recommended precision, according to wikipedia

};
