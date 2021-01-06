#include "fileloader.h"
#include "light.h"
#include "mymatrix.h"
#include "myvector.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <functional>
#include <string>
#include <vector>

#define SIZE 600

using namespace std;

Object generateCubeObject(); // Should these go in another file? Maybe fileloader.cpp?
Object generatePyramidObject();

int main() {

	Object loadedObj = loadStl("models/monkey.stl");

	// Basic SDL: https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	// Fast pixel drawing: https://stackoverflow.com/questions/33304351/
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
	}
	SDL_Window* window = SDL_CreateWindow("diy-opengl", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		SIZE, SIZE, SDL_WINDOW_SHOWN);
	if(window == nullptr){
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == nullptr){
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	// Object cubeObj = generateCubeObject();
	Object pyramidObj = generatePyramidObject();

	vector<reference_wrapper<Object>> objs{loadedObj}; // Should this store pointers/references?
	Light mylight(MyVector({0,2,2}));
	Renderer myrenderer(renderer);

	SDL_Event event;
	Uint32 fpsTimeNow = SDL_GetTicks();
	Uint32 fpsLastFrameTime = 0;
	Uint32 fpsFrameCount = 0;
	Uint32 fpsLastPrintTime = fpsTimeNow;

	while(1) {
		// Check for events
		if(SDL_PollEvent(&event) && event.type == SDL_QUIT) {
			break;
		}

		// Update
		fpsTimeNow = SDL_GetTicks();
		double deltaTime = (fpsTimeNow - fpsLastFrameTime);// / 1000.0;
		fpsLastFrameTime = fpsTimeNow;

		// cubeObj.setRotation( cubeObj.rotation().add(MyVector({0, 0.0008*deltaTime, 0})) );
		loadedObj.setRotation( loadedObj.rotation().add(MyVector({0, 0.0006*deltaTime, 0})) );

		// Draw
		myrenderer.render(objs, mylight);
		SDL_RenderPresent(renderer);

		// Calculate and print FPS
		fpsFrameCount++;
		Uint32 fpsCurrentTime = SDL_GetTicks();
		if(fpsCurrentTime - fpsLastPrintTime > 2000) {
			cout << "FPS: " << (int) ((1000.f * fpsFrameCount) / (fpsCurrentTime - fpsLastPrintTime)) << endl;
			fpsFrameCount = 0;
			fpsLastPrintTime = fpsCurrentTime;
		}

	}

	// Clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

Object generateCubeObject() {
	double cubeSize = 0.45;
	
	// http://paulbourke.net/geometry/polygonise/
	// vector<array<size_t, 3>> cubeTris = { // counter-clockwise winding
	// 	{2, 0, 1}, //face front
	// 	{3, 0, 2},
	// 	{3, 2, 4}, //face top
	// 	{4, 2, 5},
	// 	{2, 1, 5}, //face right
	// 	{5, 1, 6},
	// 	{7, 0, 4}, //face left
	// 	{4, 0, 3},
	// 	{4, 5, 7}, //face back
	// 	{7, 5, 6},
	// 	{6, 0, 7}, //face bottom
	// 	{1, 0, 6}
	// };

	// https://stackoverflow.com/questions/8142388/in-what-order-should-i-send-my-vertices-to-opengl-for-culling
	vector<MyVector> cubeVerts = {
		MyVector(vector<double>{-cubeSize,-cubeSize,cubeSize,1}),
		MyVector(vector<double>{cubeSize,-cubeSize,cubeSize,1}),
		MyVector(vector<double>{-cubeSize,cubeSize,cubeSize,1}),
		MyVector(vector<double>{cubeSize,cubeSize,cubeSize,1}),
		MyVector(vector<double>{-cubeSize,-cubeSize,-cubeSize,1}),
		MyVector(vector<double>{cubeSize,-cubeSize,-cubeSize,1}),
		MyVector(vector<double>{-cubeSize,cubeSize,-cubeSize,1}),
		MyVector(vector<double>{cubeSize,cubeSize,-cubeSize,1})
	};
	vector<array<size_t, 3>> cubeTris = {
		{0,3,2},
		{0,1,3},
		{1,7,3},
		{1,5,7},
		{5,6,7},
		{5,4,6},
		{4,2,6},
		{4,0,2},
		{4,1,0},
		{4,5,1},
		{2,7,6},
		{2,3,7}
	};

	return Object(cubeVerts, cubeTris);
}

Object generatePyramidObject() {
	double pyramidSize = 0.24;
	double pyramidOffset = 0.7; // right translation
	vector<MyVector> pyramidVerts = {
		MyVector(vector<double>{pyramidOffset-pyramidSize,0,pyramidSize,1}),
		MyVector(vector<double>{pyramidOffset+pyramidSize,0,pyramidSize,1}),
		MyVector(vector<double>{pyramidOffset+pyramidSize,0,-pyramidSize,1}),
		MyVector(vector<double>{pyramidOffset-pyramidSize,0,-pyramidSize,1}),
		MyVector(vector<double>{pyramidOffset,pyramidSize*2,0,1})
	};
	vector<array<size_t, 3>> pyramidTris = {
		{0, 1, 4},
		{1, 2, 4},
		{2, 3, 4},
		{3, 0, 4},
		{2, 1, 0},
		{0, 3, 2}
	};
	return Object(pyramidVerts, pyramidTris);
}
