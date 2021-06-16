#include "healthbar.h"
#include <algorithm>

const std::vector<uint8_t> Healthbar::HEART_PIXELS = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,2,2,2,0,0,0,2,2,2,0,0,
	0,2,1,1,1,2,0,2,1,1,1,2,0,
	2,1,1,1,1,1,2,1,1,1,1,1,2,
	2,1,1,1,1,1,1,1,1,1,1,1,2,
	2,1,1,1,1,1,1,1,1,1,1,1,2,
	0,2,1,1,1,1,1,1,1,1,1,2,0,
	0,0,2,1,1,1,1,1,1,1,2,0,0,
	0,0,0,2,1,1,1,1,1,2,0,0,0,
	0,0,0,0,2,1,1,1,2,0,0,0,0,
	0,0,0,0,0,2,1,2,0,0,0,0,0,
	0,0,0,0,0,0,2,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
};


Healthbar::Healthbar(SDL_Renderer* renderer) :
	mRenderer(renderer),
	mTextureSize(13), // Square display
	mTextureScale(8.0),
	mTextureScaledBounds({.x=30, .y=30, .w=(int)(mTextureSize*mTextureScale), .h=(int)(mTextureSize*mTextureScale)}),
	mPixels(),
	mTexture(SDL_CreateTexture(
		mRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		mTextureSize, mTextureSize
	))
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // https://wiki.libsdl.org/SDL_HINT_RENDER_SCALE_QUALITY (also set in renderer class)
	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);

	mPixels = std::vector<uint8_t>(mTextureSize * mTextureSize * 4);
	std::fill(mPixels.begin(), mPixels.end(), SDL_ALPHA_TRANSPARENT);
}

void Healthbar::render(float health) {

	for(int i=0; i<mTextureSize*mTextureSize; i++) {
		int val = HEART_PIXELS.at(i);
		if(val == 0) { // Outside heart (all pixels are transparent by default, so no action needed here)
			continue;
		} else if(val == 1) { // Inside heart
			float rowPercent = (i/mTextureSize) / (mTextureSize-2.0);
			if(rowPercent > 1 - health) { // Color is red
				mPixels[i*4 + 0] = 50;
				mPixels[i*4 + 1] = 0;
				mPixels[i*4 + 2] = 200;
				mPixels[i*4 + 3] = SDL_ALPHA_OPAQUE;
			} else { // Color is transparent
				mPixels[i*4 + 3] = SDL_ALPHA_TRANSPARENT;
			}
		} else if(val == 2) { // Outline of heart
			mPixels[i*4 + 0] = 0;
			mPixels[i*4 + 1] = 0;
			mPixels[i*4 + 2] = 0;
			mPixels[i*4 + 3] = SDL_ALPHA_OPAQUE;
		}
		
	}

	SDL_UpdateTexture (
		mTexture,
		NULL,
		mPixels.data(),
		mTextureSize * 4
	);
	SDL_RenderCopy(mRenderer, mTexture, NULL, &mTextureScaledBounds);
}
