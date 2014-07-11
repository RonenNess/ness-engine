/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer render("Hello World!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// add the hello-world sprite to it
	Ness::SpritePtr sprite = scene->create_sprite("hello_world.png");

	// loop until exit button is pressed
	bool running = true;
	Ness::Event event;
	while( running )
	{
		// fetch events and end program when getting the quit event
		while( Ness::get_poll_event( event ) != 0 )
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}
		}

		// render the scene
		render.start_frame();
		scene->render();
		render.end_frame();
	}

	// cleanup
	scene->remove(sprite);
	Ness::finish();
	return 0;
}