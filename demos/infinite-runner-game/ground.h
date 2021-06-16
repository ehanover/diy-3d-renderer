#pragma once

#include "fileloader.h"
#include "object.h"
#include <vector>
#include <functional>

struct GroundTile {
	Object object;
	bool isSafe;
};

class Ground {

public:
	Ground();

	std::vector<std::reference_wrapper<Object>> getObjects();

	void update(int deltaMS);
	bool isGroundSafe(double playerX);

private:
	int numX, numZ;
	float groundSpeed;
	size_t frontTileIndex;

	unsigned long totalMS;
	float safeGroundProbability;

	float tileSizeX, tileSizeZ;
	Object groundSafe, groundDeath;
	std::vector<GroundTile> groundObjsActive;
	GroundTile generateNewGround();

	float envSpawnProbability;
	std::vector<Object> envObjs;
	std::vector<Object> envObjsActive;
	Object generateNewEnv();

};
