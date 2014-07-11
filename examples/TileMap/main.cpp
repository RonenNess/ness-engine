/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>


int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer render("Tilemap demo!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the tilemap
	Ness::TileMapPtr map = scene->create_tilemap("tilemap.jpg", Ness::Sizei(75, 75), Ness::Size(32, 32));
	map->set_all_tiles_type(Ness::Pointi(1, 0), Ness::Sizei(6,16));

	// create the tile selection
	Ness::SpritePtr tileSelection = scene->create_sprite("tilemap.jpg");
	tileSelection->set_static(true);

	// create a camera
	Ness::CameraPtr camera = render.create_camera();
	float CameraSpeed = 500.0f;

	// keyboard handler
	Ness::Utils::Keyboard keyboard;

	// loop until exit button is pressed
	bool running = true;
	Ness::Event event;
	while( running )
	{
		// fetch events and end program when getting the quit event
		while( Ness::get_poll_event( event ) != 0 )
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}

			keyboard.inject_event(event);
		}

		// render the scene
		render.start_frame();

		// do keyboard control
		if (keyboard.ket_state(SDLK_DOWN))
		{
			camera->position.y += render.time_factor() * CameraSpeed;
		}
		if (keyboard.ket_state(SDLK_UP))
		{
			camera->position.y -= render.time_factor() * CameraSpeed;
		}
		if (keyboard.ket_state(SDLK_LEFT))
		{
			camera->position.x -= render.time_factor() * CameraSpeed;
		}
		if (keyboard.ket_state(SDLK_RIGHT))
		{
			camera->position.x += render.time_factor() * CameraSpeed;
		}
		if (keyboard.ket_state(SDLK_ESCAPE))
		{
			running = false;
		}

		// render and end the scene
		scene->render(camera);
		render.end_frame();
	}

	// cleanup
	scene->remove(tileSelection);
	scene->remove(map);
	Ness::finish();
	return 0;
}