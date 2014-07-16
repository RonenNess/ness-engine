/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "player.h"

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
	Ness::Renderer renderer("Hello World!", Ness::Sizei(800, 600), false, Ness::RENDERER_FLAG_ACCELERATED | Ness::RENDERER_FLAG_LIGHTING_NODE);

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();
	Ness::NodePtr node = scene->create_node();

	// add the hello-world sprite to it
	Ness::SpritePtr background = node->create_sprite("background.jpg");
	background->set_static(true);
	background->set_size(Ness::Sizei(renderer.get_screen_size()));

	Ness::SpritePtr floor = node->create_sprite("floor.png");
	floor->set_static(true);
	floor->set_size(Ness::Sizei(renderer.get_screen_size().x, 250));
	floor->set_anchor(Ness::Point(0.0f, 1.0f));
	floor->set_position(Ness::Pointi(0, renderer.get_screen_size().y));
	floor->set_blend_mode(Ness::BLEND_MODE_BLEND);
	
	// create the alien
	Ness::SpritePtr alien = node->create_sprite("alien.png");
	alien->set_anchor(Ness::Point::HALF);
	alien->set_position(Ness::Point(300.0f, 300.0f));
	alien->set_blend_mode(Ness::BLEND_MODE_BLEND);
	alien->set_size(Ness::Size(85, 128));
	Ness::Animators::AnimatorPtr alienAnim(new Ness::Animators::AnimatorSprite(Ness::Sizei(3, 1), 0, 2, 10.0f, Ness::Animators::SPRITE_ANIM_END_REPEAT));
	alienAnim->set_target(alien);
	renderer.register_animator(alienAnim);

	// create the light node
	Ness::LightNodePtr light = scene->create_light_node();
	light->set_ambient_color(Ness::Color(0.0f, 0.2f, 0.4f, 1.0f));

	// create light over the alien
	Ness::LightPtr alienLight = light->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::GREEN);
	alienLight->set_position(alien->get_position());
	Ness::LightPtr alienLight2 = light->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE);
	alienLight2->set_position(alien->get_position());
	alienLight2->set_scale(0.5f);

	// create the player
	Player player(node, light);

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

		// do player stuff
		player.do_events();

		// keyboard controls
		if (keyboard.ket_state(SDLK_UP))
			player.aim_light_up();
		if (keyboard.ket_state(SDLK_DOWN))
			player.aim_light_down();
		if (keyboard.ket_state(SDLK_LEFT))
			player.walk_left();
		else if (keyboard.ket_state(SDLK_RIGHT))
			player.walk_right();
		else
			player.stand();
		
		// do animations
		renderer.do_animations();

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// cleanup
	//scene->remove(sprite);
	renderer.remove_scene(scene);
	return 0;
}