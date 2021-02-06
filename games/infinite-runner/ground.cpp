#include "ground.h"

#include <cmath>

int myRandomPercent() {
	return rand() % 100;
}

Ground::Ground() :
	numX(5),
	numZ(9),
	groundSpeed(0.013),

	totalMS(0),
	safeGroundProbability(70),

	tileSizeX(3),
	tileSizeZ(6),
	groundSafe(loadStl("assets/ground_plain.stl")), // TODO model more variety for safe/dangerous ground
	groundDeath(loadStl("assets/ground_spikes.stl")),
	groundObjs()
{
	groundSafe.setColor({230, 200, 80});
	groundDeath.setColor({90, 80, 60});

	groundObjs.reserve(numX*numZ);
	for(int i=0; i<numZ; i++) {
		for(int j=0; j<numX; j++) {
			Object o = generateNewGround();
			double x = -(numX*tileSizeX)/2.0 + j*tileSizeX;
			double z = -35 + -i * tileSizeZ;
			o.setPosition({x, 0, z});
			groundObjs.push_back(o);
		}
	}
}

std::vector<std::reference_wrapper<Object>> Ground::getObjects() {
	std::vector<std::reference_wrapper<Object>> r;
	r.reserve(groundObjs.size());
	for(auto& o : groundObjs) {
		r.push_back(o);
	}
	return r;
}

void Ground::update(int deltaMS) {
	for(Object& o : groundObjs) {
		std::array<double, 3> oldPosition = o.position();

		if(oldPosition[2] > tileSizeZ + 1) {
			o = generateNewGround();
			oldPosition[2] -= numZ * tileSizeZ;
		}
		o.setPosition({oldPosition[0], oldPosition[1], oldPosition[2] + groundSpeed*deltaMS});
	}
	
	if(safeGroundProbability > 35) {
		safeGroundProbability -= 0.015;
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
