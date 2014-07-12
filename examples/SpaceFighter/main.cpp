/*
* NessEngine Depth ordering demo. creates a depth map with multiple characters and objects, illustrating the z ordering feature (znode).
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
	Ness::Renderer render("Depth ordering demo!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the background
	Ness::SpritePtr background = scene->create_sprite("background.jpg");

	// create the entities node
	Ness::NodePtr entitiesNode = scene->create_node();

	// create the player character!
	const float PlayerSpeed = 350.0f;
	const float PlayerTurningSpeed = 250.0f;
	Ness::SpritePtr player = entitiesNode->create_sprite("player.png");
	player->set_anchor(Ness::Point(0.5f, 0.5f));
	player->set_position(render.get_screen_center());
	player->set_blend_mode(Ness::BLEND_MODE_BLEND);

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

		// do keyboard control - move player around
		Ness::Point playerPos = player->get_position();
		if (keyboard.ket_state(SDLK_DOWN))
		{
			playerPos.y += render.time_factor() * PlayerSpeed;
		}
		if (keyboard.ket_state(SDLK_UP))
		{
			playerPos.y -= render.time_factor() * PlayerSpeed;
		}
		if (keyboard.ket_state(SDLK_LEFT))
		{
			player->set_rotation(player->get_rotation() - render.time_factor() * PlayerTurningSpeed);
		}
		if (keyboard.ket_state(SDLK_RIGHT))
		{
			player->set_rotation(player->get_rotation() + render.time_factor() * PlayerTurningSpeed);
		}
		if (keyboard.ket_state(SDLK_ESCAPE))
		{
			g_running = false;
		}
		player->set_position(playerPos);

		// render and end the scene
		scene->render();
		render.end_frame();
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(entitiesNode);
	scene->remove(background);
	entitiesNode->remove(player);
	Ness::finish();
	return 0;
}