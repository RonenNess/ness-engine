/*
* This example shows how to create a custom sprite object with 3d lighting effect.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "sprite3d.h"

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
	Ness::Renderer renderer("3d lighting!", Ness::Sizei(1200, 1100), false, Ness::RENDERER_FLAG_ACCELERATED | Ness::RENDERER_FLAG_LIGHTING_NODE);

	// set the color key we'll be using for our textures
	renderer.resources().set_color_key(Ness::Colorb(255,0,255,255));

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create the node that will contain all the objects
	Ness::NodePtr node = scene->create_node();

	// create the light node
	// note: it's created after 'node' to make sure the lighting will be above all objects and affect them
	Ness::LightNodePtr lightNode = scene->create_light_node();
	lightNode->set_ambient_color(Ness::Color(0.1f, 0.15f, 0.25f));
	lightNode->set_visible(true);

	// create some lights
	Ness::LightPtr light1 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE);
	light1->set_position(Ness::Point(200, 200));

	//Ness::LightPtr light2 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::RED);
	//light2->set_position(Ness::Point(600, 400));
	//light2->set_visible(false);

	// create the sprite with 3d lighting effects
	Ness::SpritePtr sprite = ness_make_ptr<Sprite3d>(lightNode, "deamon.png");
	sprite->set_position(Ness::Point(400, 400));
	scene->add(sprite);

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::Mouse Mouse;
	EventsPoller.add_handler(Mouse);

	// loop until exit button is pressed
	while( g_running )
	{
		// handle events
		EventsPoller.poll_events(HandleEvents, false);

		// update 3d sprite position
		sprite->set_position(Mouse.position());

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// cleanup
	scene->remove(sprite);
	renderer.remove_scene(scene);
	return 0;
}