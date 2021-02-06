#pragma once

#include "fileloader.h"
#include "object.h"
#include <vector>
#include <functional>

class Player {

public:
	Player();

	std::vector<std::reference_wrapper<Object>> getObjects();

	void update(int deltaMS);
	void jump();

	bool isMovingLeft;
	bool isMovingRight;

private:
	std::array<double, 3> hatPos, headPos, torsoPos, armRPos, armLPos, legRPos, legLPos;
	Object hat, head, torso, armR, armL, legR, legL;

	bool isGrounded;
	unsigned long walkMS;

	std::array<double, 3> playerPosition;
	float yVel;
	float yAcc;

};
