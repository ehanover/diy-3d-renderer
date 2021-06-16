/*
cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cmake --install . --prefix ".." && ./infinite-runner

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-pg .. && make && cmake --install . --prefix ".." && timeout 10 ./infinite-runner
*/


#include "ground.h"
#include "player.h"

#include "camera.h"
#include "fileloader.h"
#include "healthbar.h"
#include "light.h"
#include "mymatrix.h"
#include "myvector.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <chrono>
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <vector>

using namespace std;

int main() {
	// const int WINDOW_HEIGHT = 750;
	// const int WINDOW_WIDTH = (int) (WINDOW_HEIGHT * (4.0/3));
	// const double RENDERER_SCALE = 0.35;
	const int WINDOW_HEIGHT = 700;
	const int WINDOW_WIDTH = (int) (WINDOW_HEIGHT * (16.0/9));
	const double RENDERER_SCALE = 0.45; // 1.0 = each final pixel is rendered by the renderer, there's no upscaling
	
	// Fast pixel drawing: https://stackoverflow.com/questions/33304351/
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
	}
	SDL_Window* window = SDL_CreateWindow("infinite-runner", 
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

	vector<uint8_t> mybackground;
	mybackground.reserve(WINDOW_WIDTH*WINDOW_HEIGHT*RENDERER_SCALE*4);
	float skyHeight = 0.219686; // Dumb hardcoded number so that the horizon is a straight line
	int skyPixels = mybackground.capacity()*skyHeight;
	for(int i=0; i<skyPixels/4; i++) {
		mybackground.push_back(245); // BGR
		mybackground.push_back(152);
		mybackground.push_back(56);
		mybackground.push_back(SDL_ALPHA_OPAQUE);
	}
	for(int i=0; i<(mybackground.capacity() - skyPixels)/4; i++) {
		mybackground.push_back(77);
		mybackground.push_back(155);
		mybackground.push_back(185);
		mybackground.push_back(SDL_ALPHA_OPAQUE);
	}

	Camera mycamera(0, 5, 12, 0, 4, 0, 0, 1, 0);
	Light mylight(4,5,4); // TODO experiment with light position
	Renderer myrenderer(renderer, RENDERER_SCALE, mycamera, mybackground, 2, -6*9);
	Healthbar healthbar(renderer);

	Player player;
	Ground ground;

	SDL_Event event;

	Uint32 fpsTimeNow = SDL_GetTicks();
	Uint32 fpsLastFrameTime = 0;
	Uint32 fpsFrameCount = 0;
	Uint32 fpsLastPrintTime = fpsTimeNow;
	unsigned long ticks = 0;
	unsigned int deltaMS;
	Uint32 startTime = fpsTimeNow; // Used for final printing of score

	while(true) {
		// Find delta time
		fpsTimeNow = SDL_GetTicks();
		deltaMS = (fpsTimeNow - fpsLastFrameTime);
		if(deltaMS < 17) { // Limit FPS to around 60
			this_thread::sleep_for(chrono::milliseconds(17 - deltaMS));

			fpsTimeNow = SDL_GetTicks();
			deltaMS = (fpsTimeNow - fpsLastFrameTime);
		}
		fpsLastFrameTime = fpsTimeNow;

		// Events
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				break;
			} else if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_SPACE) {
					player.jump();
				} else if(event.key.keysym.sym == SDLK_LEFT) {
					player.isMovingLeft = true;
					player.isMovingRight = false;
				} else if(event.key.keysym.sym == SDLK_RIGHT) {
					player.isMovingLeft = false;
					player.isMovingRight = true;
				}
			} else if(event.type == SDL_KEYUP) {
				if(event.key.keysym.sym == SDLK_LEFT) {
					player.isMovingLeft = false;
				} else if(event.key.keysym.sym == SDLK_RIGHT) {
					player.isMovingRight = false;
				}
			}
		}

		// Update
		player.update(deltaMS);
		ground.update(deltaMS);

		if(ground.isGroundSafe(player.playerPosition[0]) == false && player.playerPosition[1] < 0.75) {
			player.isHurting = true;
			player.health -= 0.00035 * deltaMS;
			// cout << "ow! health=" << player.health << endl;
		} else {
			player.isHurting = false;
		}

		// Render
		vector<reference_wrapper<Object>> objs;
		for(Object& o : ground.getObjects()) {
			objs.push_back(o);
		}
		for(Object& o : player.getObjects()) {
			objs.push_back(o);
		}
		myrenderer.render(objs, mylight);
		healthbar.render(player.health);
		SDL_RenderPresent(renderer);

		if(player.health <= 0) { // This is belongs in the update section, but it works better here because the player should look red upon dying
			// It would also be cool to add a special pose for the player when he dies
			this_thread::sleep_for(chrono::milliseconds(1500)); // Freeze the game for a bit when you die
			break;
		}

		// Calculate and print FPS
		fpsFrameCount++;
		Uint32 fpsCurrentTime = SDL_GetTicks();
		if(fpsCurrentTime - fpsLastPrintTime > 2000) {
			cout << "FPS: " << (int) ((1000.f * fpsFrameCount) / (fpsCurrentTime - fpsLastPrintTime)) << endl;
			fpsFrameCount = 0;
			fpsLastPrintTime = fpsCurrentTime;
		}

		ticks += 1;
	}

	// Clean up
	cout << "You survived for " << (SDL_GetTicks() - startTime)/1000 << " seconds. Thanks for playing!" << endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
