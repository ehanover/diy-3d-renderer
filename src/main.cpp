#include "mymatrix.h"
#include "myvector.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <functional>
#include <string>
#include <vector>

#define SIZE 350

using namespace std;

int main() {

	// http://paulbourke.net/geometry/polygonise/
	double cubeSize = 0.4;
	vector<double> cubeVerts = {
		-cubeSize,-cubeSize,-cubeSize,1,
		cubeSize,-cubeSize,-cubeSize,1,
		cubeSize,-cubeSize,cubeSize,1,
		-cubeSize,-cubeSize,cubeSize,1,
		-cubeSize,cubeSize,-cubeSize,1,
		cubeSize,cubeSize,-cubeSize,1,
		cubeSize,cubeSize,cubeSize,1,
		-cubeSize,cubeSize,cubeSize,1
	};
	vector<array<size_t, 3>> cubeTris = {
		{0, 3, 2},
		{1, 0, 2},
		{5, 6, 1},
		{6, 2, 1},
		{4, 7, 6},
		{5, 4, 6},
		{7, 4, 0},
		{3, 7, 0},
		{4, 5, 0},
		{5, 1, 0},
		{6, 7, 3},
		// {6, 2, 3} // reverse, cw
		{2, 6, 3}
	};
	Object cubeObj(MyMatrix(8, 4, cubeVerts), cubeTris);

	double pyramidSize = 0.24;
	double pyramidOffset = 0.7;
	vector<double> pyramidVerts = {
		pyramidOffset-pyramidSize,0,pyramidSize,1,
		pyramidOffset-pyramidSize,0,-pyramidSize,1,
		pyramidOffset+pyramidSize,0,-pyramidSize,1,
		pyramidOffset+pyramidSize,0,pyramidSize,1,
		pyramidOffset+0,pyramidSize*3,0,1
	};
	vector<array<size_t, 3>> pyramidTris = {
		// {3, 1, 4} // reverse, cw
		// {4, 1, 3}
	};
	Object pyramidObj(MyMatrix(5, 4, pyramidVerts), pyramidTris);

	// Basic SDL: https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	// Fast pixel drawing: https://stackoverflow.com/questions/33304351/
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
	}
	SDL_Window* window = SDL_CreateWindow("My Window", 
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

	vector<reference_wrapper<Object>> objs{cubeObj}; // Should this store pointers/references?
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

		cubeObj.setRotation( cubeObj.rotation().add(MyVector({0, 0.0005*deltaTime, 0})) );

		// Draw
		SDL_SetRenderDrawColor(renderer, 0x30, 0x40, 0x50, 0xFF);
		SDL_RenderClear(renderer);
		myrenderer.render(objs);
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
