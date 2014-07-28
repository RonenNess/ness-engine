/*
* NessEngine StaticNode demo. static node is a node for entities that do not change after creation, which take advantage of the canvas entity to create great one-time-drawing optimization.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

void make_random_colors(const Ness::Pointi& index, const Ness::SpritePtr& tile)
{
	tile->set_color(Ness::Color::get_random().get_limit(0.5f, 1.0f));
}

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer render("StaticNode demo!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the static node
	Ness::StaticNodePtr staticNode = scene->create_static_node();

	// create the tilemap
	const int TilemapSize = 256;
	const int TileSize = 32;
	Ness::TileMapPtr map = staticNode->create_tilemap("tilemap.jpg", Ness::Sizei(TilemapSize, TilemapSize), Ness::Sizei(TileSize, TileSize));
	map->apply_to_all(make_random_colors);

	// create random trees
	const int NumberOfTrees = 5000;
	for (int i = 0; i < NumberOfTrees; i++)
	{
		Ness::SpritePtr tree = staticNode->create_sprite("tree.png");
		tree->set_anchor(Ness::Point::HALF);
		tree->set_blend_mode(Ness::BLEND_MODE_BLEND);
		tree->set_position(Ness::Pointi(rand() % (TilemapSize * TileSize), rand() % (TilemapSize * TileSize)));
	}

	// build the static node
	staticNode->build();

	// create instructions text
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", 
		"use arrows to move. there are currently " + ness_int_to_string((TilemapSize * TilemapSize) + NumberOfTrees) + " static objects!", 16);
	instructions->set_position(Ness::Point(0.0f, (float)render.get_screen_size().y));
	instructions->set_anchor(Ness::Point(0.0f, 1.0f));

	// create fps text
	Ness::TextPtr fps = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "FPS", 24);

	// create a camera
	Ness::CameraPtr camera = render.create_camera();
	float CameraSpeed = 500.0f;

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::Mouse mouse;
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);
	EventsPoller.add_handler(app);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// render the scene
		render.start_frame();

		// do keyboard control
		if (keyboard.key_state(SDLK_DOWN))
		{
			camera->position.y += render.time_factor() * CameraSpeed;
		}
		if (keyboard.key_state(SDLK_UP))
		{
			camera->position.y -= render.time_factor() * CameraSpeed;
		}
		if (keyboard.key_state(SDLK_LEFT))
		{
			camera->position.x -= render.time_factor() * CameraSpeed;
		}
		if (keyboard.key_state(SDLK_RIGHT))
		{
			camera->position.x += render.time_factor() * CameraSpeed;
		}

		// pick the tile we currently point on with the mouse
		Ness::SpritePtr tile = map->get_sprite_by_position((Ness::Pointi)camera->position + mouse.position());

		// render and end the scene
		scene->render(camera);
		render.end_frame();

		// set fps
		fps->change_text("fps: " + ness_int_to_string(render.fps()));
	}

	return 0;
}