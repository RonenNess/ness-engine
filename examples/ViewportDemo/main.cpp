/*
* NessEngine Depth ordering demo. creates a depth map with multiple characters and objects, illustrating the z ordering feature (znode).
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

// number of random objects
const int NumOfObjects = 500;

// resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Viewport demo!", Ness::Sizei(SCREEN_WIDTH, SCREEN_HEIGHT), Ness::DEFAULT_WINDOW_FLAGS, Ness::DEFAULT_RENDERER_FLAGS | Ness::RENDERER_FLAG_VSYNC);

	// create a new scene and another alternative scene for the undead
	Ness::ScenePtr scene = renderer.create_scene();
	Ness::ScenePtr dead_scene = renderer.create_scene();

	// create the viewport
	Ness::ViewportPtr viewport = renderer.create_viewport();
	viewport->set_anchor(Ness::Point::HALF);
	viewport->set_mask("mask.jpg");

	// create the tilemap
	const int TileSize = 64;
	const int TileMapSize = 100;
	const int TotalMapSize = (TileSize * TileMapSize);
	Ness::TileMapPtr map = scene->create_tilemap("tilemap.jpg", Ness::Sizei(TileMapSize, TileMapSize), Ness::Sizei(TileSize, TileSize));
	Ness::TileMapPtr dead_map = dead_scene->create_tilemap("tilemap_dead.jpg", Ness::Sizei(TileMapSize, TileMapSize), Ness::Sizei(TileSize, TileSize));

	// create the znode - a depth-based ordering scene node
	Ness::NodePtr znode = scene->create_znode();
	Ness::NodePtr dead_znode = dead_scene->create_znode();

	// create a camera
	Ness::CameraPtr camera = renderer.create_camera();
	float PlayerSpeed = 250.0f;

	// create random trees and rocks
	for (int i = 0; i < NumOfObjects; i++)
	{
		Ness::SpritePtr object;
		object = znode->create_sprite( i < 200 ? "tree.png" : "rock.png");
		object->set_anchor(Ness::Point(0.5f, 1.0f));
		object->set_position(Ness::Pointi(rand() % TotalMapSize, rand() % TotalMapSize));
		object->set_zindex(object->get_position().y);
		object->set_blend_mode(Ness::BLEND_MODE_BLEND);

		Ness::SpritePtr dead_object;
		dead_object = dead_znode->create_sprite( i < 200 ? "tree_dead.png" : "rock_dead.png");
		dead_object->set_anchor(Ness::Point(0.5f, 1.0f));
		dead_object->set_position(object->get_position());
		dead_object->set_zindex(dead_object->get_position().y);
		dead_object->set_blend_mode(Ness::BLEND_MODE_BLEND);
	}

	// create the player character!
	Ness::SpritePtr player = znode->create_sprite("fighter.png");
	player->set_anchor(Ness::Point(0.5f, 1.0f));
	player->set_position(Ness::Pointi(TotalMapSize / 2, TotalMapSize / 2));
	player->set_blend_mode(Ness::BLEND_MODE_BLEND);

	Ness::SpritePtr dead_player = dead_znode->create_sprite("fighter_dead.png");
	dead_player->set_anchor(Ness::Point(0.5f, 1.0f));
	dead_player->set_position(Ness::Pointi(TotalMapSize / 2, TotalMapSize / 2));
	dead_player->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// create the fps show
	Ness::TextPtr fpsShow = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "fps", 20);

	// create instructions text
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "use arrows to move around and see z-ordering in action.", 20);
	instructions->set_position(Ness::Point(0, 24));

	// create the corner logo
	Ness::SpritePtr corner_logo = scene->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
	corner_logo->set_static(true);
	corner_logo->set_position(renderer.get_screen_size());

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::Mouse mouse;
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);
	EventsPoller.add_handler(app);

	// show logo screen
	//Ness::Utils::make_logo_screen(scene, "../ness-engine/resources/gfx/logo.png");

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// render the scene
		renderer.start_frame();

		// do keyboard control - move player around
		Ness::Point playerPos = player->get_position();
		if (keyboard.key_state(SDLK_DOWN))
		{
			playerPos.y += renderer.time_factor() * PlayerSpeed;
		}
		if (keyboard.key_state(SDLK_UP))
		{
			playerPos.y -= renderer.time_factor() * PlayerSpeed;
		}
		if (keyboard.key_state(SDLK_LEFT))
		{
			playerPos.x -= renderer.time_factor() * PlayerSpeed;
		}
		if (keyboard.key_state(SDLK_RIGHT))
		{
			playerPos.x += renderer.time_factor() * PlayerSpeed;
		}
		player->set_position(playerPos);
		dead_player->set_position(playerPos);

		// set the viewport position based on mouse
		viewport->set_source_position(-(renderer.get_screen_center() - mouse.position()));
		viewport->set_dest_position(mouse.position());

		// fix player zorder based on his y position
		player->set_zindex(playerPos.y);
		dead_player->set_zindex(playerPos.y);

		// set camera to focus on player
		camera->position.x = player->get_position().x - (renderer.get_screen_size().x * 0.5f);
		camera->position.y = player->get_position().y - (renderer.get_screen_size().y * 0.5f);

		// render and end the scene
		scene->render(camera);
		dead_scene->render_on_viewport(viewport, camera);
		viewport->render();
		renderer.end_frame();

		// update fps show
		Ness::String FpsShow = Ness::String("fps ") + (renderer.get_flags() & Ness::RENDERER_FLAG_VSYNC ? "(vsync): " : ": ");
		fpsShow->change_text(FpsShow + ness_int_to_string(renderer.fps()));
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(map);
	return 0;
}