#pragma once

#include "camera.h"
#include "mymatrix.h"
#include "object.h"
#include <SDL2/SDL.h>
#include <array>
#include <functional>
#include <vector>

class Renderer {

public:
	Renderer(SDL_Renderer* renderer);

	void render(const std::vector<std::reference_wrapper<Object>>& objs);

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
	void fakeFragmentShader();
	std::array<double, 2> getCoordFromVert(const MyVector& v);
	double edgeFunction(const std::array<double, 2>& a, const std::array<double, 2>& b, const std::array<double, 2>& c);
	double facingRatio(const MyVector& norm);

	SDL_Renderer* mRenderer;
	int mTextureSizeX;
	int mTextureSizeY;
	SDL_Texture* mTexture;

	Camera mCamera;

	// MyMatrix mTransformationMat;
	// MyMatrix mProjectionMat;
	double mPerspectiveDist;
	MyMatrix mPerspectiveMat;

	std::vector<MyVector> mVertsRender;
	std::vector<std::array<size_t, 3>> mTrisRender;
	std::vector<MyVector> mNormsRender;
	// std::vector<int> mColorsRender;
	std::vector<unsigned char> mPixels;

};
