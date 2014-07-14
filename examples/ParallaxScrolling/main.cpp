/*
* NessEngine ParallaxScrolling demo - illustrates how to easily create parallax scrolling technique using a camera object.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

// is the program still running
bool g_running = true;

// resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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
	Ness::Renderer renderer("Parallax Scrolling", Ness::Sizei(SCREEN_WIDTH, SCREEN_HEIGHT));

	// nearest background layer
	Ness::ScenePtr layer1 = renderer.create_scene();
	Ness::SpritePtr background1 = layer1->create_sprite("layer1.png");
	background1->set_anchor(Ness::Point(0.0f, 1.0f));
	background1->set_blend_mode(Ness::BLEND_MODE_BLEND);
	background1->set_position(Ness::Point(0, SCREEN_HEIGHT));

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;

	// loop until exit button is pressed
	while( g_running )
	{
		// handle events
		EventsPoller.poll_events(HandleEvents, false);

		// render the scene
		renderer.start_frame();
		layer1->render();
		renderer.end_frame();
	}

	// cleanup
	renderer.remove_scene(layer1);
	Ness::finish();
	return 0;
}