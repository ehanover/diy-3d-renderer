#include "mymatrix.h"
#include "myvector.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <string>
#include <vector>

#define SIZE 600

using namespace std;

int main() {

	double cubeSize = 0.2;
	// http://paulbourke.net/geometry/polygonise/
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
	MyMatrix cubeMatrix(8, 4, cubeVerts);

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

	// vector<double> projectionVec{
	// 	camAxisX.elem(0), camAxisY.elem(0), camAxisZ.elem(0), 0, 
	// 	camAxisX.elem(1), camAxisY.elem(1), camAxisZ.elem(1), 0, 
	// 	camAxisX.elem(2), camAxisY.elem(2), camAxisZ.elem(2), 0, 
	// 	-1*(camAxisX.dot(camEye)), -1*(camAxisY.dot(camEye)), -1*(camAxisZ.dot(camEye)), 1
	// };
	// MyMatrix projectionMat(4, 4, projectionVec);
	// cout << projectionMat << endl;

	// Perspective
	// MyMatrix cubePerspective = cubeTransformed;





	// return 0;
	
	sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "My window");
	window.setFramerateLimit(30);

	while (window.isOpen()) {
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Drawing
		window.clear(sf::Color::Black);

		sf::RectangleShape boundsRect(sf::Vector2f(SIZE, SIZE));
		boundsRect.setPosition(0, 0);
		boundsRect.setFillColor(sf::Color(30,40,50));
		window.draw(boundsRect);

		for(int i=0; i<12; i++) {

		}
		
		window.display();
	}
	return 0;
}
