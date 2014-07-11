#include "NessEngine.h"

namespace Ness
{
	// MUST BE CALLED BEFORE USING THE ENGINE.
	void init(int imgFlags)
	{
		// initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			throw FailedToInitRenderer(SDL_GetError());
		}

		// initialize image loading
		if ( IMG_Init( imgFlags ) != imgFlags )
		{
			throw FailedToInitRenderer(SDL_GetError());
		}
	}

	// should be called at the end to cleanup stuff.
	void finish()
	{
		IMG_Quit();
		SDL_Quit();
	}
};