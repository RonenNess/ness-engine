/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
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
	Ness::Renderer render("Tilemap demo!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = render.create_scene();

	// create the tilemap
	const int TileSize = 32;							// <-- 32 is the size of a single tile in the spritesheet
	const Ness::Pointi TilesInSpritesheet(6, 16);		// <-- 6 is how many tile types we got in the spritesheet on X axis, 16 is how many we got on Y axis
	Ness::TileMapPtr map = scene->create_tilemap("tilemap.jpg", Ness::Sizei(100, 100), Ness::Sizei(TileSize, TileSize));
	map->set_all_tiles_type(Ness::Pointi(0, 0), TilesInSpritesheet);

	// create the highlight of the currently selected tile
	Ness::RectangleShapePtr selectedTile = scene->create_rectangle();
	selectedTile->set_size(Ness::Sizei(TileSize, TileSize));
	selectedTile->set_color(Ness::Color::RED);
	selectedTile->set_anchor(Ness::Point(0.5f, 1.0f));
	selectedTile->set_filled(false);

	// create the tile selection box
	Ness::SpritePtr tileSelection = scene->create_sprite("tilemap.jpg");
	tileSelection->set_static(true);

	// create the tile selection red border
	Ness::RectangleShapePtr border = scene->create_rectangle();
	border->set_size(tileSelection->get_size());
	border->set_color(Ness::Color::RED);
	border->set_static(true);
	border->set_filled(false);

	// create the highlight of the selected tile from the toolbar on the left
	Ness::RectangleShapePtr selectedType = scene->create_rectangle();
	selectedType->set_size(Ness::Sizei(TileSize, TileSize));
	selectedType->set_color(Ness::Color(1.0f, 0.0f, 0.0f, 0.45f));
	selectedType->set_static(true);
	selectedType->set_filled(true);
	selectedType->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// the currently selected tile type to set
	Ness::Pointi SelectedTileType(0, 0);

	// create a camera
	Ness::CameraPtr camera = render.create_camera();
	float CameraSpeed = 500.0f;

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
			g_running = false;
		}

		// pick the tile we currently point on with the mouse
		Ness::SpritePtr tile = map->get_sprite_by_position((Ness::Pointi)camera->position + mouse.position());

		// if a tile is picked set the visual on it
		if (tile)
		{
			selectedTile->set_position(tile->get_position());
			selectedTile->set_visible(true);
		}
		else
		{
			selectedTile->set_visible(false);
		}
		

		// if left mouse click is down:
		if (mouse.is_down(Ness::Utils::MOUSE_LEFT))
		{
			// if mouse position is inside the tile selection, we pick a new tile type:
			if (mouse.position().x <= tileSelection->get_size().x && mouse.position().y <= tileSelection->get_size().y)
			{
				SelectedTileType.x = (int)(mouse.position().x / TileSize);
				SelectedTileType.y = (int)(mouse.position().y / TileSize);
				selectedType->set_position(SelectedTileType * TileSize);
			}

			// else, if we got selected tile, change its type
			else if (tile)
			{
				tile->set_source_from_sprite_sheet(SelectedTileType, TilesInSpritesheet);
			}
		}

		// render and end the scene
		scene->render(camera);
		render.end_frame();
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(border);
	scene->remove(tileSelection);
	scene->remove(map);
	Ness::finish();
	return 0;
}