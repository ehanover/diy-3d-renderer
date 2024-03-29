#include "camera.h"
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

using namespace std;


int main() {
	const int WINDOW_HEIGHT = 400;
	const int WINDOW_WIDTH = (int) (WINDOW_HEIGHT * (4.0/3));
	const double RENDERER_SCALE = 1.0;
	
	Object loadedObj = loadStl("assets/monkey.stl");

	// Basic SDL: https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
	// Fast pixel drawing: https://stackoverflow.com/questions/33304351/
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
	}
	SDL_Window* window = SDL_CreateWindow("engine-test", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		SDL_WINDOW_SHOWN);
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

	Camera mycamera(0, 1, 4, 0, 0, 0, 0, 1, 0);
	Light mylight(0, 1, 4);
	vector<reference_wrapper<Object>> objs{pyramidObj, loadedObj}; // Should this store pointers/references?
	Renderer myrenderer(renderer, RENDERER_SCALE, mycamera, vector<uint8_t>(), 3, -3);

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
		int deltaTime = (fpsTimeNow - fpsLastFrameTime);
		fpsLastFrameTime = fpsTimeNow;

		pyramidObj.setRotation( {pyramidObj.rotation()[0] + -0.0015*deltaTime, pyramidObj.rotation()[1] + -0.0008*deltaTime, 0} );
		loadedObj.setRotation( {0, loadedObj.rotation()[1] + 0.0006*deltaTime, 0} );

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
