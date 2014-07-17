/*
* Shows how to use the multiline text entity
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

// is the program still running
bool g_running = true;

// callback to handle exit events
void HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_QUIT)
		g_running = false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Multiline Text!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create some texts
	Ness::MultiTextPtr text1 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"this is first line.\nthis is second line.\nlast line!!!", 20);

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;

	// loop until exit button is pressed
	while( g_running )
	{
		// handle events
		EventsPoller.poll_events(HandleEvents, false);

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// cleanup
	renderer.remove_scene(scene);
	return 0;
}