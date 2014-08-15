/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

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