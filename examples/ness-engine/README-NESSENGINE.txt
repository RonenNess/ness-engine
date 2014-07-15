NESSENGINE README

1. Ness-Engine is a scene-oriented 2d rendering engine, designed especially for rapid games development.
It's built upon the SDL2 lib.

2. Ness-Engine is distributed under the zlib license: http://www.zlib.net/zlib_license.html

2. Ness-Engine contains several libraries it is built upon (SDL2, SDL_image, SDL_ttf), most of them distributed with the zlib license as well, all of them free for commercial use. inside the runtime directory for your platform (for example 'lib/windows/x86/') you will find a readme file for every library with it's specific license.

3. To run properly, make sure the folder 'include' is inside the include directories of your project. you must be able to include files from there using the "<filename.h>" syntax.

4. The lib dir contains both dlls and lib files. you need to include ness_engine.lib in your project, and make sure ALL the dll files are accessible for runtime.  you may include the other lib files if you want to use SDL components directly.

5. source code can be found here: https://github.com/RonenNess/ness-engine

6. Please distribute this file with the your project (not mandatory)

Enjoy!
- Ronen Ness, 2014