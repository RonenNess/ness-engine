/*
* NessEngine Depth ordering demo. creates a depth map with multiple characters and objects, illustrating the z ordering feature (znode).
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "player.h"

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
	Ness::Renderer render("SpaceFighter demo!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the background
	Ness::SpritePtr background = scene->create_sprite("background.jpg");

	// create the entities node
	Ness::NodePtr entitiesNode = scene->create_node();

	// creating player 1 (defined in player.h)
	Player player1(entitiesNode, "player1.png");
	player1.set_position(render.get_screen_center() + Ness::Point(0, 100));
	player1.set_rotation(90);

	// creating player 2
	Player player2(entitiesNode, "player2.png");
	player2.set_position(render.get_screen_center() + Ness::Point(0, -100));
	player2.set_rotation(-90);

	// test meteor
	Ness::SpritePtr Meteor = scene->create_sprite("meteor.png");
	Meteor->set_anchor(Ness::Point(0.5f, 0.5f));
	Meteor->set_blend_mode(Ness::BLEND_MODE_BLEND);
	Meteor->set_position(Ness::Point(200, 200));

	// create the event handlers
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::Mouse mouse;
	Ness::Utils::EventsPoller EventsPoller;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);

	// loop until exit button is pressed
	while( g_running )
	{
		// handle events
		EventsPoller.poll_events(HandleEvents, false);

		// render the scene
		render.start_frame();

		// do players events
		player1.do_events();
		player2.do_events();

		// KEYBOARD CONTROL:

		// player1 controls
		if (keyboard.ket_state(SDLK_UP))
			player1.fly_forward();
		if (keyboard.ket_state(SDLK_DOWN))
			player1.fly_backwards();
		if (keyboard.ket_state(SDLK_LEFT))
			player1.turn_left();
		if (keyboard.ket_state(SDLK_RIGHT))
			player1.turn_right();

		// player2 controls
		if (keyboard.ket_state(SDLK_w))
			player2.fly_forward();
		if (keyboard.ket_state(SDLK_s))
			player2.fly_backwards();
		if (keyboard.ket_state(SDLK_a))
			player2.turn_left();
		if (keyboard.ket_state(SDLK_d))
			player2.turn_right();

		// exit
		if (keyboard.ket_state(SDLK_ESCAPE))
		{
			g_running = false;
		}

		// render and end the scene
		scene->render();
		render.end_frame();
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(entitiesNode);
	scene->remove(background);
	Ness::finish();
	return 0;
}