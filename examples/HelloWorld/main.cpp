/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/
#define _WINDOWS
#include <NessEngine.h>
#include <tchar.h>

#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Hello World!", Ness::Sizei(512, 512));

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// add the hello-world sprite to it
	Ness::SpritePtr sprite = scene->create_sprite("hello_world.png");

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(app);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// finish
	return 0;
}