/*
* An RPG demo project.
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

// number of random objects
const int NumOfObjects = 500;

// resolution
#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 1100

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
	Ness::Renderer render("Depth ordering demo!", Ness::Sizei(SCREEN_WIDTH, SCREEN_HEIGHT));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the tilemap
	const int TileSize = 64;
	const int TileMapSize = 100;
	const int TotalMapSize = (TileSize * TileMapSize);
	Ness::TileMapPtr map = scene->create_tilemap("gfx/tilemap.jpg", Ness::Sizei(TileMapSize, TileMapSize), Ness::Sizei(TileSize, TileSize));

	// create the znode - a depth-based ordering scene node
	Ness::NodePtr znode = scene->create_znode();

	// create the light node
	Ness::LightNodePtr lightNode = scene->create_light_node();
	lightNode->set_ambient_color(Ness::Color(0.5f, 0.5f, 0.6f));
	Ness::LightPtr light1 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE);
	light1->set_scale(Ness::Point(2.0f, 1.75f));

	// create a camera
	Ness::CameraPtr camera = render.create_camera();
	float PlayerSpeed = 250.0f;

	// create random trees and rocks
	for (int i = 0; i < NumOfObjects; i++)
	{
		Ness::SpritePtr object;
		object = Ness::SpritePtr(new Sprite3d(lightNode, i < 200 ? "gfx/tree.png" : "gfx/rock.png"));
		znode->add(object);
		object->set_anchor(Ness::Point(0.5f, 1.0f));
		object->set_position(Ness::Pointi(rand() % TotalMapSize, rand() % TotalMapSize));
		object->set_zindex(object->get_position().y);
		object->set_blend_mode(Ness::BLEND_MODE_BLEND);
	}

	// create the player character!
	Ness::SpritePtr player = znode->create_sprite("gfx/fighter.png");
	player->set_anchor(Ness::Point(0.5f, 1.0f));
	player->set_position(Ness::Pointi(TotalMapSize / 2, TotalMapSize / 2));
	player->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// create the event handlers
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::Mouse mouse;
	Ness::Utils::EventsPoller EventsPoller;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);

	// create the fps show
	Ness::TextPtr fpsShow = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "fps", 20);

	// create instructions text
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "use arrows to move around and see z-ordering in action.", 20);
	instructions->set_position(Ness::Point(0, 24));

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
		light1->set_position(playerPos - Ness::Point(0.0f, player->get_absolute_size().y * 0.25f));

		// fix player zorder based on his y position
		player->set_zindex(playerPos.y);
		light1->set_zindex(playerPos.y);

		// set camera to focus on player
		camera->position.x = player->get_position().x - (render.get_screen_size().x * 0.5f);
		camera->position.y = player->get_position().y - (render.get_screen_size().y * 0.5f) - player->get_absolute_size().y * 0.5f;

		// render scene and end frame
		scene->render(camera);
		render.end_frame();

		// update fps show
		std::string FpsShow = std::string("fps ") + (render.get_flags() & Ness::RENDERER_FLAG_VSYNC ? "(vsync): " : ": ");
		fpsShow->change_text(FpsShow + std::to_string((long long)render.fps()));
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(map);
	return 0;
}