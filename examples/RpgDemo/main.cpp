/*
* RPG demo project.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "sprite3d.h"
#include "isoTilemap.h"
#include "character.h"

// is the program still running
bool g_running = true;

// number of random objects
const int NumOfObjects = 500;

// resolution
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

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
	Ness::Renderer render("Wolf RPG demo!", Ness::Sizei(SCREEN_WIDTH, SCREEN_HEIGHT));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the tilemap
	const int TileSize = 128;
	const int TileMapSize = 100;
	const int TotalMapSize = (TileSize * (TileMapSize - 1));
	Ness::TileMapPtr map = ness_make_ptr<IsoTilemap>(&render, "gfx/tilemap.jpg", Ness::Sizei(TileMapSize, TileMapSize), Ness::Sizei(TileSize, TileSize));
	scene->add(map);

	// create the znode - a depth-based ordering scene node
	Ness::NodePtr znode = scene->create_znode();

	// create the light node
	Ness::LightNodePtr lightNode = scene->create_light_node();
	lightNode->set_ambient_color(Ness::Color(0.45f, 0.45f, 0.56f));

	// create a camera
	Ness::CameraPtr camera = render.create_camera();
	float PlayerSpeed = 250.0f;

	// create random trees and rocks
	for (int i = 0; i < NumOfObjects; i++)
	{
		int type = rand() % 4;
		Ness::SpritePtr object;
		switch (type)
		{
		case 0:
			object = ness_make_ptr<Sprite3d>(lightNode, "gfx/rock.png");
			break;
		case 1:
			object = ness_make_ptr<Sprite3d>(lightNode, "gfx/bush.png");
			break;
		default:
			object = ness_make_ptr<Sprite3d>(lightNode, "gfx/tree.png");
			break;
		}
		znode->add(object);
		object->set_anchor(Ness::Point(0.5f, 1.0f));
		object->set_position(Ness::Pointi(rand() % TotalMapSize, rand() % TotalMapSize));
		object->set_zindex(object->get_position().y);
		object->set_blend_mode(Ness::BLEND_MODE_BLEND);
	}

	// create the player character!
	NessSharedPtr<Character> player = ness_make_ptr<Character>(lightNode, znode, "gfx/wolf.png");
	render.register_animator(player);
	player->set_position(Ness::Pointi(TotalMapSize / 2, TotalMapSize / 2));
	
	// add the character sprite (just a guy standing in the forest, nothing serious)
	NessSharedPtr<Character> wizard = ness_make_ptr<Character>(lightNode, znode, "gfx/player.png");
	render.register_animator(wizard);
	wizard->set_position(Ness::Pointi(TotalMapSize / 2 + 100, TotalMapSize / 2));
	wizard->get_light()->set_scale(3.0f);
	wizard->get_light()->set_color(Ness::Color::GREEN);

	// create the event handlers
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::Mouse mouse;
	Ness::Utils::EventsPoller EventsPoller;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);

	// create the fps show
	Ness::TextPtr fpsShow = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "fps", 20);

	// create instructions text
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "use arrows to move around, space to shoot fireball", 20);
	instructions->set_position(Ness::Point(0, 24));

	// loop until exit button is pressed
	while( g_running )
	{
		// handle events
		EventsPoller.poll_events(HandleEvents, false);

		// render the scene
		render.start_frame();

		// do keyboard control - move player around
		if (keyboard.ket_state(SDLK_DOWN))
		{
			player->walk(DIRECTION_DOWN);
		}
		else if (keyboard.ket_state(SDLK_UP))
		{
			player->walk(DIRECTION_UP);
		}
		else if (keyboard.ket_state(SDLK_LEFT))
		{
			player->walk(DIRECTION_LEFT);
		}
		else if (keyboard.ket_state(SDLK_RIGHT))
		{
			player->walk(DIRECTION_RIGHT);
		}
		else
		{
			player->stop();
		}
		if (keyboard.ket_state(SDLK_SPACE))
		{
			player->shoot();
		}
		if (keyboard.ket_state(SDLK_ESCAPE))
		{
			g_running = false;
		}

		// set camera to focus on player
		camera->position.x = player->get_position().x - (render.get_screen_size().x * 0.5f);
		camera->position.y = player->get_position().y - (render.get_screen_size().y * 0.5f) - player->get_absolute_size().y * 0.5f;

		// fix camera position
		camera->position.limitx(0, TotalMapSize - SCREEN_WIDTH);
		camera->position.limity(0, TotalMapSize - SCREEN_HEIGHT);

		// render scene and end frame
		scene->render(camera);
		render.end_frame();

		// update fps show
		std::string FpsShow = std::string("fps ") + (render.get_flags() & Ness::RENDERER_FLAG_VSYNC ? "(vsync): " : ": ");
		fpsShow->change_text(FpsShow + std::to_string((long long)render.fps()));
	}

	return 0;
}