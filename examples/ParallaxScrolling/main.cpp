/*
* NessEngine ParallaxScrolling demo - illustrates how to easily create parallax scrolling technique using a camera object.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include <time.h>

// resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	srand ((unsigned int)time(NULL));

	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Parallax Scrolling", Ness::Sizei(SCREEN_WIDTH, SCREEN_HEIGHT));

	float scale = 1.25f;

	// nearest background layer
	Ness::ScenePtr layer1 = renderer.create_scene();
	Ness::SpritePtr background1 = layer1->create_sprite("layer1.png");
	background1->set_anchor(Ness::Point(0.0f, 1.0f));
	background1->set_blend_mode(Ness::BLEND_MODE_BLEND);
	background1->set_position(Ness::Point(0, SCREEN_HEIGHT));
	background1->set_scale(scale);

	// middle background layer
	Ness::ScenePtr layer2 = renderer.create_scene();
	Ness::SpritePtr background2 = layer2->create_sprite("layer2.png");
	background2->set_anchor(Ness::Point(0.0f, 0.5f));
	background2->set_blend_mode(Ness::BLEND_MODE_BLEND);
	background2->set_position(Ness::Point(0, SCREEN_HEIGHT * 0.55f));
	background2->set_scale(scale);

	// farest background layer
	Ness::ScenePtr layer3 = renderer.create_scene();
	Ness::SpritePtr background3 = layer3->create_sprite("layer3.png");
	background3->set_anchor(Ness::Point(0.0f, 0.5f));
	background3->set_blend_mode(Ness::BLEND_MODE_BLEND);
	background3->set_position(Ness::Point(0, 0));
	background3->set_scale(scale);

	// calc the screen width limit
	const float MaxScreenWidth = background1->get_size().x * scale;

	// create the super front layer that hides everything
	Ness::ScenePtr front = renderer.create_scene();
	for (unsigned int i = 0; i < 3; i++)
	{
		Ness::SpritePtr palm = front->create_sprite("palm.png");
		palm->set_anchor(Ness::Point(0.0f, 1.0f));
		palm->set_blend_mode(Ness::BLEND_MODE_BLEND);
		palm->set_position(Ness::Point((float)(rand() % (int)MaxScreenWidth), SCREEN_HEIGHT));
		palm->set_scale(2.0f + ((rand() % 10) / 5.0f));
		if (rand() % 10 <= 5)
		{
			palm->flip_x();
		}
	}

	// create instructions text
	Ness::TextPtr instructions = layer1->create_text("../ness-engine/resources/fonts/courier.ttf", "use arrows to move around and see the parallax scrolling in action.", 20);
	instructions->set_color(Ness::Color::BLACK);
	instructions->set_static(false);

	// create camera
	Ness::CameraPtr camera = renderer.create_camera();

	// create the corner logo
	Ness::SpritePtr corner_logo = layer1->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
	corner_logo->set_static(true);
	corner_logo->set_position(renderer.get_screen_size());

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(keyboard);
	EventsPoller.add_handler(app);

	// show logo screen
	Ness::Utils::make_logo_screen(front, "../ness-engine/resources/gfx/logo.png");

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// move camera
		float cameraSpeed = 100.0f;
		if (keyboard.key_state(SDLK_UP))
			camera->position.y -= renderer.time_factor() * cameraSpeed;
		if (keyboard.key_state(SDLK_DOWN))
			camera->position.y += renderer.time_factor() * cameraSpeed;
		if (keyboard.key_state(SDLK_LEFT))
			camera->position.x -= renderer.time_factor() * cameraSpeed;
		if (keyboard.key_state(SDLK_RIGHT))
			camera->position.x += renderer.time_factor() * cameraSpeed;

		// make sure camera is at legal position
		float MaxX = MaxScreenWidth - SCREEN_WIDTH;
		float MinX = 0.0f;
		camera->position.limitx(MinX, MaxX);
		float MinY = -60.0f;
		float MaxY = 0.0f;
		camera->position.limity(MinY, MaxY);

		// store camera original position and start frame
		Ness::Point camPos = camera->position;
		renderer.start_frame();

		// render farest layer
		camera->position *= 0.25f;
		layer3->render(camera);
		camera->position = camPos;

		// render middle layer
		camera->position *= 0.5f;
		layer2->render(camera);
		camera->position = camPos;

		// render front layer
		layer1->render(camera);

		// render super front layer
		camera->position *= 1.5f;
		front->render(camera);
		camera->position = camPos;

		// end frame
		renderer.end_frame();
	}

	// cleanup
	renderer.remove_scene(layer1);
	renderer.remove_scene(layer2);
	renderer.remove_scene(layer3);
	renderer.remove_scene(front);
	return 0;
}