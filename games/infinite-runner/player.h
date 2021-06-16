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
	bool isHurting;

	float health;
	std::array<double, 3> playerPosition;

private:
	std::array<double, 3> hatPos, headPos, torsoPos, armRPos, armLPos, legRPos, legLPos;
	Object hat, head, torso, armR, armL, legR, legL;
	std::array<uint8_t, 3> colorSkin;
	std::array<uint8_t, 3> colorSkinHurt;

	bool isGrounded;
	unsigned long walkMS;

	float yVel;
	float yAcc;

};
