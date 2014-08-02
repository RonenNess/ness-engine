/*
* This example shows the lighting node, a special type of node that take advantage of the canvas entity to create lighting mask.
* You can use the lighting node for all your basic lighting needs, or you can easily create your own lighting system using canvas and other entities.
* See example "3dLighting" for more advanced lighting techniques.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "player.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Lighting Demo", Ness::Sizei(800, 600), false, Ness::RENDERER_FLAG_ACCELERATED | Ness::RENDERER_FLAG_LIGHTING_NODE);

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
	Ness::Animators::AnimatorPtr alienAnim = ness_make_ptr<Ness::Animators::AnimatorSprite>(alien, Ness::Sizei(3, 1), 0, 3, 5.0f, Ness::Animators::SPRITE_ANIM_END_REPEAT);
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

	// create instructions text
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "use arrows to move and aim flashlight", 22);
	instructions->set_color(Ness::Color(1.0f, 1.0f, 1.0f, 0.5f));

	// create the player
	Player player(node, light);

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::Mouse mouse;
	Ness::Utils::Keyboard keyboard;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(mouse);
	EventsPoller.add_handler(keyboard);
	EventsPoller.add_handler(app);

	// show logo screen
	Ness::Utils::make_logo_screen(scene, "../ness-engine/resources/gfx/logo.png");

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// do player stuff
		player.do_events();

		// keyboard controls
		if (keyboard.key_state(SDLK_UP))
			player.aim_light_up();
		if (keyboard.key_state(SDLK_DOWN))
			player.aim_light_down();
		if (keyboard.key_state(SDLK_LEFT))
			player.walk_left();
		else if (keyboard.key_state(SDLK_RIGHT))
			player.walk_right();
		else
			player.stand();

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// cleanup
	renderer.remove_scene(scene);
	return 0;
}