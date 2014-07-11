/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

// TESTING /////////////////////////////////////////////
#include "NessEngine\NessEngine.h"
////////////////////////////////////////////////////////

void test(const Ness::Pointi& index, const Ness::SpritePtr& sprite)
{
	//sprite->set_color(Ness::Color::get_random());
}

int main( int argc, char* args[] )
{
	// init the engine
	Ness::init();

	// create the renderer
	Ness::Renderer render("test", Ness::Sizei(1920, 1200), false, Ness::RENDERER_FLAG_ACCELERATED | Ness::RENDERER_FLAG_TARGET_TEXTURE);

	// create scene, node and camera
	Ness::ScenePtr scene = render.create_scene();
	Ness::NodePtr node = scene->create_znode();
	Ness::CameraPtr camera = render.create_camera();

	// set defaults
	Ness::Sprite::Defaults.anchor = Ness::Point(0.5f, 1.0f);
	Ness::Sprite::Defaults.alpha_channels = true;

	// create the tilemap
	Ness::TileMapPtr map = node->create_tilemap("test.jpg", Ness::Sizei(200, 200), Ness::Size(64, 64));
	map->set_all_tiles_type(Ness::Pointi(1, 0), Ness::Sizei(16,15));

	// create random fighters
	for (int i = 0; i < 10000; i++)
	{
		Ness::SpritePtr fighter = node->create_sprite("fighter.png");
		fighter->set_position(Ness::Point((float)(rand() % 10000), (float)(rand() % 10000)));
		fighter->set_zindex((int)fighter->get_position().y);
		fighter->set_color(Ness::Color::get_random());
	}

	// create the fighter
	Ness::SpritePtr fighter = node->create_sprite("fighter.png");

	#if 0
	// create canvas
	Ness::CanvasPtr screen_canvas = scene->create_canvas("tilemap");
	screen_canvas->set_static(true);
	screen_canvas->auto_clean(true);
	screen_canvas->set_anchor(Ness::Point::HALF);
	screen_canvas->set_position(render.get_target_center());
	float zoom = 1.0f;

	scene->remove(screen_canvas);
	scene->add(screen_canvas);

	// set the tile map target to be our new canvas
	node->set_render_target(screen_canvas->get_texture());
	#endif

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//node->set_scale(Ness::Point(2, 2));
	//screen_canvas->set_rotation(42);

	// keyboard wrapper
	Ness::Utils::Keyboard keyboard;

	float camspeed = 200.0f;

	//While application is running
	while( !quit )
	{


		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			switch( e.type ){

			case SDL_QUIT:
				quit = true;
				break;
			}

			keyboard.inject_event(e);
			
		}


		// START FRAME
		render.start_frame();

		//screen_canvas->clear();
		//node->set_color(Ness::Color::get_random());

		// render!
		scene->render(camera);

		// do keyboard
		if (keyboard.ket_state(SDLK_LEFT))
			camera->position.x -= render.time_factor() * camspeed;
		if (keyboard.ket_state(SDLK_RIGHT))
			camera->position.x += render.time_factor() * camspeed;
		if (keyboard.ket_state(SDLK_UP))
			camera->position.y -= render.time_factor() * camspeed;
		if (keyboard.ket_state(SDLK_DOWN))
			camera->position.y += render.time_factor() * camspeed;
		if (keyboard.ket_state(SDLK_ESCAPE))
			quit = true;

		fighter->set_position(camera->position + render.get_screen_center());
		fighter->set_zindex((int)fighter->get_position().y);

		#if 0
		if (keyboard.ket_state(SDLK_1))
			zoom += render.time_factor();
		if (keyboard.ket_state(SDLK_2))
			zoom -= render.time_factor();

		screen_canvas->set_scale(Ness::Point(zoom,zoom));
		#endif

		// END FRAME
		render.end_frame();
		render.set_window_title(std::to_string((long long)render.fps()).c_str());
	}

	Ness::finish();
	return 0;
}