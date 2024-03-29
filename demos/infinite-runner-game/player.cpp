#include "player.h"

#include <cmath>

Player::Player() :

	isMovingLeft(false),
	isMovingRight(false),
	isHurting(false),

	hatPos({0, 0, 0}),
	headPos({0, 0, 0}),
	torsoPos({0, 0, 0}),
	armRPos({0, 2.1, 0}),
	armLPos({-1, 2.1, 0}),
	legRPos({0, 1.2, 0}),
	legLPos({-0.49, 1.2, 0}),

	hat(loadStl("assets/person_hat.stl")),
	head(loadStl("assets/person_head.stl")),
	torso(loadStl("assets/person_torso.stl")),
	armR(loadStl("assets/person_arm.stl")),
	armL(armR),
	legR(loadStl("assets/person_leg.stl")),
	legL(legR),
	colorSkin({230, 200, 150}),
	colorSkinHurt({210, 90, 70}),

	health(1.0),
	isGrounded(true),
	walkMS(0),

	playerPosition({0, 0, 0.9}),
	yVel(0),
	yAcc(0)
{
	std::array<uint8_t, 3> colorClothes = {150, 100, 80};
	std::array<uint8_t, 3> colorClothesDark = {70, 50, 40};
	hat.setColor(colorClothes);
	hat.setPosition(hatPos);
	head.setColor(colorSkin);
	head.setPosition(headPos);
	torso.setColor(colorClothes);
	torso.setPosition(torsoPos);
	armR.setColor(colorSkin);
	armR.setPosition(armRPos);
	armL.setColor(colorSkin);
	armL.setPosition(armLPos);
	legR.setColor(colorClothesDark);
	legR.setPosition(legRPos);
	legL.setColor(colorClothesDark);
	legL.setPosition(legLPos);
}

std::vector<std::reference_wrapper<Object>> Player::getObjects() {
	return std::vector<std::reference_wrapper<Object>>{hat, head, torso, armL, armR, legL, legR};
}

void Player::update(int deltaMS) {
	if(isGrounded) {
		walkMS += deltaMS;
		float animValL = std::sin((walkMS)/110.0) * 0.8;
		float animValR = -1 * animValL;
		legL.setRotation(std::array<double, 3>{-3.14 + 0.0 + animValL, 0, 0});
		legR.setRotation(std::array<double, 3>{-3.14 + 0.0 + animValR, 0, 0});

		armL.setRotation({-3.14 + -0.3 + animValR * 0.7, 0, 0});
		armR.setRotation({-3.14 + -0.3 + animValL * 0.7, 0, 0});
	} else {
		playerPosition[1] += yVel * deltaMS;
		yVel += yAcc * deltaMS;

		if(playerPosition[1] <= 0) {
			isGrounded = true;
			playerPosition[1] = 0;
			yVel = 0;
			yAcc = 0;
		}

	}

	if(isMovingLeft && playerPosition[0] > -7) {
		playerPosition[0] += -0.007 * deltaMS;
	} else if(isMovingRight && playerPosition[0] < 7) {
		playerPosition[0] += 0.007 * deltaMS;
	}

	if(isHurting) {
		head.setColor(colorSkinHurt);
		armL.setColor(colorSkinHurt);
		armR.setColor(colorSkinHurt);
	} else {
		head.setColor(colorSkin);
		armL.setColor(colorSkin);
		armR.setColor(colorSkin);
	}

	hat.setPosition({hatPos[0] + playerPosition[0], hatPos[1] + playerPosition[1]*1.1, hatPos[2] + playerPosition[2]});
	head.setPosition({headPos[0] + playerPosition[0], headPos[1] + playerPosition[1], headPos[2] + playerPosition[2]});
	torso.setPosition({torsoPos[0] + playerPosition[0], torsoPos[1] + playerPosition[1], torsoPos[2] + playerPosition[2]});
	armL.setPosition({armLPos[0] + playerPosition[0], armLPos[1] + playerPosition[1], armLPos[2] + playerPosition[2]});
	armR.setPosition({armRPos[0] + playerPosition[0], armRPos[1] + playerPosition[1], armRPos[2] + playerPosition[2]});
	legL.setPosition({legLPos[0] + playerPosition[0], legLPos[1] + playerPosition[1], legLPos[2] + playerPosition[2]});
	legR.setPosition({legRPos[0] + playerPosition[0], legRPos[1] + playerPosition[1], legRPos[2] + playerPosition[2]});

}

void Player::jump() {
	if(isGrounded == false) {
		return;
	}
	isGrounded = false;
	yAcc = -0.00004;
	yVel = 0.017; // (This velocity value gets multiplied by deltaMS before it's added to position)

}
