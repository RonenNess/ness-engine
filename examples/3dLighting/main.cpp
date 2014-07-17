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
	Ness::Renderer renderer("3d lighting!", Ness::Sizei(800, 600), false, Ness::RENDERER_FLAG_ACCELERATED | Ness::RENDERER_FLAG_LIGHTING_NODE);
	renderer.set_background_color(Ness::Color::WHITE);

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create the node that will contain all the objects
	Ness::NodePtr node = scene->create_node();

	// create the light node
	// note: it's created after 'node' to make sure the lighting will be above all objects and affect them
	Ness::LightNodePtr lightNode = scene->create_light_node();
	lightNode->set_ambient_color(Ness::Color(0.1f, 0.1f, 0.15f));
	lightNode->set_visible(true);

	// create some lights
	Ness::LightPtr light1 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE);
	light1->set_position(Ness::Point(400, 300));
	light1->set_scale(1.5f);

	// create the sprite with 3d lighting effects
	NessSharedPtr<Sprite3d> sprite1 = ness_make_ptr<Sprite3d>(lightNode, "deamon.png");
	sprite1->set_position(Ness::Point(400, 400));
	sprite1->set_scale(1.5f);
	node->add(sprite1);
	NessSharedPtr<Sprite3d> sprite2 = ness_make_ptr<Sprite3d>(lightNode, "deamon.png");
	sprite2->set_position(Ness::Point(400, 400));
	sprite2->set_scale(1.5f);
	sprite2->render_diffuse(false);
	sprite2->set_visible(false);
	node->add(sprite2);

	// add instructions
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "mouse: move the object, 1-4: change objects, 5-6: change lights", 20);
	instructions->set_color(Ness::Color(1.0f, 1.0f, 1.0f, 0.5f));

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::Mouse mouse;
	Ness::Utils::Keyboard keyboard;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);

	// loop until exit button is pressed
	while( g_running )
	{
		// handle events
		EventsPoller.poll_events(HandleEvents, false);

		// update 3d sprite position
		sprite1->set_position(mouse.position());
		sprite2->set_position(mouse.position());
		//sprite3->set_position(mouse.position());
		//sprite4->set_position(mouse.position());

		// keyboard control - toggle objects
		if (keyboard.ket_state(SDLK_1))
		{
			sprite1->set_visible(true);
			sprite2->set_visible(false);
		}
		if (keyboard.ket_state(SDLK_2))
		{
			sprite1->set_visible(false);
			sprite2->set_visible(true);
		}

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// cleanup
	renderer.remove_scene(scene);
	return 0;
}