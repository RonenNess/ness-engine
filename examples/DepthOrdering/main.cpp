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

	// create the tilemap
	const int TileSize = 64;
	const int TileMapSize = 100;
	Ness::TileMapPtr map = scene->create_tilemap("tilemap.jpg", Ness::Sizei(TileMapSize, TileMapSize), Ness::Sizei(TileSize, TileSize));

	// create the znode - a depth-based ordering scene node
	Ness::NodePtr znode = scene->create_znode();

	// create a camera
	Ness::CameraPtr camera = render.create_camera();
	float PlayerSpeed = 250.0f;

	// create random 1000 trees
	for (int i = 0; i < 1000; i++)
	{
		Ness::SpritePtr tree = znode->create_sprite("tree.png");
		tree->set_anchor(Ness::Point(0.5f, 1.0f));
		tree->set_position(Ness::Pointi(rand() % (TileSize * TileMapSize), rand() % (TileSize * TileMapSize)));
		tree->set_zindex(tree->get_position().y);
		tree->set_scale(1.0f + rand());
		tree->set_blend_mode(Ness::BLEND_MODE_BLEND);
	}

	// create the player character!
	Ness::SpritePtr player = znode->create_sprite("fighter.png");
	player->set_anchor(Ness::Point(0.5f, 1.0f));
	player->set_position(Ness::Pointi(800, 800));
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
			playerPos.x -= render.time_factor() * PlayerSpeed;
		}
		if (keyboard.ket_state(SDLK_RIGHT))
		{
			playerPos.x += render.time_factor() * PlayerSpeed;
		}
		if (keyboard.ket_state(SDLK_ESCAPE))
		{
			g_running = false;
		}
		player->set_position(playerPos);

		// fix player zorder based on his y position
		player->set_zindex(playerPos.y);

		// set camera to focus on player
		camera->position.x = player->get_position().x - (render.get_screen_size().x * 0.5f);
		camera->position.y = player->get_position().y - (render.get_screen_size().y * 0.5f);

		// render and end the scene
		scene->render(camera);
		render.end_frame();
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(map);
	Ness::finish();
	return 0;
}