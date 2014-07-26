#include "NessEngine.h"

namespace Ness
{
	bool g_is_ness_init = false;

	// MUST BE CALLED BEFORE USING THE ENGINE.
	void init(int imgFlags)
	{
		// initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			throw FailedToInitLib("SDL", SDL_GetError());
		}

		// init the SDL_ttf
		if (TTF_Init() < 0)
		{
			throw FailedToInitLib("SDL_TTF", TTF_GetError());
		}

		// initialize image loading
		if ( IMG_Init( imgFlags ) != imgFlags )
		{
			throw FailedToInitLib("SDL_IMAGE", IMG_GetError());
		}

		// register the finish function on exit (in case user forgets to call it)
		g_is_ness_init = true;
		atexit(__finish);
	}

	// get next event
	bool poll_event(Event& event)
	{
		return (SDL_PollEvent( &event ) != 0 );
	}

	// should be called at the end to cleanup stuff.
	void __finish()
	{
		if (!g_is_ness_init) return;
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		g_is_ness_init = false;
	}
};