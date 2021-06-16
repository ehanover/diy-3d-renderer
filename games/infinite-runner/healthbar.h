#pragma once

#include <SDL2/SDL.h>
#include <vector>

class Healthbar {

public:
	Healthbar(SDL_Renderer* renderer);

	void render(float health);

private:
	static const std::vector<uint8_t> HEART_PIXELS;

	int mTextureSize;
	float mTextureScale;

	SDL_Renderer* mRenderer;
	SDL_Rect mTextureScaledBounds;
	std::vector<uint8_t> mPixels;
	SDL_Texture* mTexture;

};
