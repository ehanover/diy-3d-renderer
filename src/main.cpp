#include "mymatrix.h"
#include "myvector.h"

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
	double verts[8][3] = {
		{-cubeSize,-cubeSize,-cubeSize},
		{cubeSize,-cubeSize,-cubeSize},
		{cubeSize,-cubeSize,cubeSize},
		{-cubeSize,-cubeSize,cubeSize},
		{-cubeSize,cubeSize,-cubeSize},
		{cubeSize,cubeSize,-cubeSize},
		{cubeSize,cubeSize,cubeSize},
		{-cubeSize,cubeSize,cubeSize},
	};
	/* int edges[12][2] = {
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
	vector<double> cubeVerts;
	for(int i=0; i<8; i++) {
		for(int j=0; j<3; j++) {
			cubeVerts.push_back(verts[i][j]);
		}
		cubeVerts.push_back(1);
	}
	MyMatrix cubeMat(8, 4, cubeVerts);

	// Transformation
	// MyMatrix cubeTransformed = cubeMatrix;

	// Projection
	MyVector camEye(3, vector<double>{2, 3, 3});
	MyVector camAt(3, vector<double>{0, 0, 0});
	MyVector camUp(3, vector<double>{0, 1, 0});
	MyVector camAxisZ = camAt.add(camEye.scalar(-1)).normalize();
	MyVector camAxisX = camUp.cross(camAxisZ).normalize();
	MyVector camAxisY = camAxisZ.cross(camAxisX).normalize();

	cout << "camAxisZ=" << camAxisZ << endl;
	cout << "camAxisX=" << camAxisX << endl;
	cout << "camAxisY=" << camAxisY << endl;
	cout << endl;

	vector<double> projectionVec{
		camAxisX.elem(0), camAxisY.elem(0), camAxisZ.elem(0), 0, 
		camAxisX.elem(1), camAxisY.elem(1), camAxisZ.elem(1), 0, 
		camAxisX.elem(2), camAxisY.elem(2), camAxisZ.elem(2), 0, 
		-1*(camAxisX.dot(camEye)), -1*(camAxisY.dot(camEye)), -1*(camAxisZ.dot(camEye)), 1
	};
	MyMatrix projectionMat(4, 4, projectionVec);
	MyMatrix cubeMatProjected = cubeMat.multiply(projectionMat);

	// Perspective
	double d = 1;
	vector<double> perspectiveVec{
		SIZE, 0, 0, 0,
		0, SIZE, 0, 0,
		0, 0, 1, (1/d),
		0, 0, 0, 0
	};
	MyMatrix perspectiveMat(4, 4, perspectiveVec);
	MyMatrix cubeMatPerspective = cubeMatProjected.multiply(perspectiveMat);
	// return 0;
	




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

		for(int i=0; i<12; i++) {
			double w = cubeMatPerspective.data()[4*i + 3];
			double x = (1 * (cubeMatPerspective.data()[4*i + 0]/w)) + (SIZE/2);
			double y = (1 * (cubeMatPerspective.data()[4*i + 1]/w)) + (SIZE/2);

			SDL_Rect fillRect{(int)x, (int)y, 5, 5};
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x80, 0xFF);
			SDL_RenderFillRect(renderer, &fillRect);
		}
		SDL_RenderPresent(renderer);

		// Calculate and print FPS
		fpsFrameCount++;
		Uint32 fpsCurrentTime = SDL_GetTicks();
		if(fpsLastTime < fpsCurrentTime - 1500) {
			cout << "FPS: " << (1000.f * fpsFrameCount) / (fpsCurrentTime - fpsLastTime) << endl;
			fpsFrameCount = 0;
			fpsLastTime = fpsCurrentTime;
		}

	}

	// Clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
