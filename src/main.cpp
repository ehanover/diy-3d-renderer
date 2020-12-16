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

int main() {

	// http://paulbourke.net/geometry/polygonise/
	double cubeSize = 0.5;
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
	vector<array<size_t, 3>> cubeTris;
	Object cubeObj(MyMatrix(8, 4, cubeVerts), cubeTris);

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

	Renderer myrenderer(renderer);
	vector<reference_wrapper<Object>> objs{cubeObj}; // Should this store pointers/references?

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

		// break;

	}

	// Clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
