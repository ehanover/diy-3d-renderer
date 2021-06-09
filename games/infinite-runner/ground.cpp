#include "ground.h"

#include <cmath>

int myRandomPercent() {
	return rand() % 100;
}

std::array<uint8_t, 3> groundDeathColorFunc(const MyVector& v) {
	if(v.elem(1) > 0.5) {
		return {180, 180, 180}; // Top of spikes
	}
	if(v.elem(1) < 0) {
		return {70, 60, 60}; // Bottom of spikes
	}
	// if(v.elem(0) > 2.9 || v.elem(0) < 0.1 || -v.elem(2) > 5.9 || -v.elem(2) < 0.1) {
	// 	return {240, 200, 100}; // Edge of tile
	// }
	// return {210, 170, 70};  // Center of tile

	return {240, 200, 100};
}

Ground::Ground() :
	numX(5),
	numZ(9),
	groundSpeed(0.013),

	totalMS(0),
	safeGroundProbability(70),

	tileSizeX(3),
	tileSizeZ(6),
	groundSafe(loadStl("assets/ground_plain.stl")), // Maybe I should model more variety for safe/dangerous ground
	groundDeath(loadStl("assets/ground_spikes.stl")),
	groundObjsActive(),

	envSpawnProbability(85), // Probability to spawn in a row
	envObjs({loadStl("assets/cactus1.stl"), loadStl("assets/cactus2.stl")}),
	envObjsActive()
{
	groundSafe.setColor({240, 200, 100});
	// groundDeath.setColor({90, 80, 60});
	groundDeath.setColorFunction(&groundDeathColorFunc);

	envObjs[0].setColor({80, 150, 50});
	envObjs[1].setColor({60, 130, 30});

	groundObjsActive.reserve(numX*numZ);
	for(int i=0; i<numZ; i++) {
		double z = -35 + -i * tileSizeZ;
		for(int j=0; j<numX; j++) {
			Object o = generateNewGround();
			double x = -(numX*tileSizeX)/2.0 + j*tileSizeX;
			o.setPosition({x, 0, z});
			groundObjsActive.push_back(o);
		}
		if(myRandomPercent() < envSpawnProbability) {
			Object o = generateNewEnv();
			o.setPosition({o.position()[0], 0, z});
			envObjsActive.push_back(o);
		}
	}
}

std::vector<std::reference_wrapper<Object>> Ground::getObjects() {
	std::vector<std::reference_wrapper<Object>> r;
	r.reserve(groundObjsActive.size() + envObjsActive.size());
	for(auto& o : groundObjsActive) {
		r.push_back(o);
	}
	for(auto& o : envObjsActive) {
		r.push_back(o);
	}
	return r;
}

void Ground::update(int deltaMS) {
	bool rowOffScreen = false;
	for(Object& o : groundObjsActive) {
		std::array<double, 3> oldPosition = o.position();

		if(oldPosition[2] > tileSizeZ + 1) {
			rowOffScreen = true;
			o = generateNewGround();
			oldPosition[2] -= numZ * tileSizeZ;
		}
		o.setPosition({oldPosition[0], oldPosition[1], oldPosition[2] + groundSpeed*deltaMS});
	}
	for(std::vector<Object>::iterator it=envObjsActive.begin(); it!=envObjsActive.end();) {
		std::array<double, 3> oldPosition = it->position();
		if(oldPosition[2] > 1) {
			envObjsActive.erase(it);
		} else {
			it->setPosition({oldPosition[0], oldPosition[1], oldPosition[2] + groundSpeed*deltaMS});
			it++;
		}
	}

	if(rowOffScreen) {
		if(myRandomPercent() < envSpawnProbability) {
			Object o = generateNewEnv();
			o.setPosition({o.position()[0], 0, -numZ * tileSizeZ});
			envObjsActive.push_back(o);
		}
	}
	
	if(safeGroundProbability > 35) {
		safeGroundProbability -= 0.014;
	}
	if(groundSpeed < 0.028) {
		groundSpeed += 0.000008;
	}

}

bool Ground::isGroundSafe(double playerX) {
	return true;
}

Object Ground::generateNewGround() {
	if(myRandomPercent() < safeGroundProbability) { // The ground is a safe tile
		Object o = groundSafe;
		return o;
	} else {  // The ground is a dangerous tile
		Object o = groundDeath;
		return o;
	}
}

Object Ground::generateNewEnv() {
	float minX = numX * (tileSizeX + 1) / 2.0;
	float maxX = 50;
	float r = myRandomPercent()/100.0;
	double x = (minX + (maxX-minX)*r) * (myRandomPercent() > 50 ? 1 : -1);
	Object o = envObjs.at(myRandomPercent() % envObjs.size());
	o.setPosition({x, 0, 0});
	o.setRotation({0, myRandomPercent()/16.0, 0});
	return o;
}
