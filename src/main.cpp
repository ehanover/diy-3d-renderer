#include "mymatrix.h"
#include "myvector.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <string>
#include <vector>

#define SIZE 600

using namespace std;

int main() {

	// http://paulbourke.net/geometry/polygonise/
	double cubeSize = 0.5;
	vector<array<double, 4>> cubeVerts = {
		{-cubeSize,-cubeSize,-cubeSize,1},
		{cubeSize,-cubeSize,-cubeSize,1},
		{cubeSize,-cubeSize,cubeSize,1},
		{-cubeSize,-cubeSize,cubeSize,1},
		{-cubeSize,cubeSize,-cubeSize,1},
		{cubeSize,cubeSize,-cubeSize,1},
		{cubeSize,cubeSize,cubeSize,1},
		{-cubeSize,cubeSize,cubeSize,1}
	};
	/* int cubeEdges[12][2] = {
		{0,1},
		{1,2},
		{2,3},
		{3,0},
		{4,5},
		{5,6},
		{6,7},
		{7,4},
		{4,0},
		{5,1},
		{6,2},
		{7,3}
	}; */
	vector<array<size_t, 3>> cubeTris;
	Object cubeObj(cubeVerts, cubeTris);
	

	// https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	// Look at https://stackoverflow.com/questions/33304351/ for fast pixel drawing
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
	vector<Object> objs{cubeObj};

	SDL_Event event;
	Uint32 fpsFrameCount = 0;
	Uint32 fpsLastTime = SDL_GetTicks();

	while(1) {
		// Check for events
		if(SDL_PollEvent(&event) && event.type == SDL_QUIT) {
			break;
		}

		// Draw
		SDL_SetRenderDrawColor(renderer, 0x30, 0x40, 0x50, 0xFF);
		SDL_RenderClear(renderer);

		myrenderer.render(objs);

		SDL_RenderPresent(renderer);

		// TODO calculate deltaTime for use in transformations

		// Calculate and print FPS
		fpsFrameCount++;
		Uint32 fpsCurrentTime = SDL_GetTicks();
		if(fpsCurrentTime - fpsLastTime > 2000) {
			cout << "FPS: " << (1000.f * fpsFrameCount) / (fpsCurrentTime - fpsLastTime) << endl;
			fpsFrameCount = 0;
			fpsLastTime = fpsCurrentTime;
		}
		// break;

	}

	// Clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
