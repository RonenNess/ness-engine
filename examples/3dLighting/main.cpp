/*
* This example shows how to create a custom sprite object with 3d lighting effect.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "sprite3d.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("3d lighting!", Ness::Sizei(800, 600), false, Ness::RENDERER_FLAG_ACCELERATED | Ness::RENDERER_FLAG_LIGHTING_NODE);
	renderer.set_background_color(Ness::Color::WHITE);

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create the node that will contain all the objects
	Ness::NodePtr node = scene->create_node();
	node->set_position(renderer.get_screen_center());

	// create the light node
	// note: it's created after 'node' to make sure the lighting will be above all objects and affect them
	Ness::LightNodePtr lightNode = scene->create_light_node();
	lightNode->set_ambient_color(Ness::Color(0.1f, 0.1f, 0.15f));
	lightNode->set_visible(true);

	// create some lights
	Ness::LightPtr light1 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE);
	light1->set_scale(1.5f);
	light1->set_position(renderer.get_screen_size() / 2);
	Ness::LightPtr light2 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color(1.0f, 0.4f, 0.4f));
	light2->set_scale(2.0f);
	light2->set_visible(false);
	light2->set_position(Ness::Point(0.0f, 300.0f));
	Ness::LightPtr light3 = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color(0.4f, 0.4f, 1.0f));
	light3->set_scale(2.0f);
	light3->set_visible(false);
	light3->set_position(Ness::Point(800.0f, 300.0f));

	// sprite 1: monster with diffuse
	SharedPtr<Sprite3d> sprite1 = ness_make_ptr<Sprite3d>(lightNode, "deamon.png");
	sprite1->set_scale(1.5f);
	node->add(sprite1);

	// sprite 2: monster without diffuse
	SharedPtr<Sprite3d> sprite2 = ness_make_ptr<Sprite3d>(lightNode, "deamon.png");
	sprite2->set_scale(1.5f);
	sprite2->render_diffuse(false);
	sprite2->set_visible(false);
	node->add(sprite2);

	// sprite 3: rock
	SharedPtr<Sprite3d> sprite3 = ness_make_ptr<Sprite3d>(lightNode, "rock.png", false);
	sprite3->set_scale(1.5f);
	sprite3->set_visible(false);
	node->add(sprite3);

	// sprite 4: rock without diffuse
	SharedPtr<Sprite3d> sprite4 = ness_make_ptr<Sprite3d>(lightNode, "rock.png", false);
	sprite4->set_scale(1.5f);
	sprite4->render_diffuse(false);
	sprite4->set_visible(false);
	node->add(sprite4);

	// add instructions
	Ness::TextPtr instructions = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "mouse: move the object, 1-4: change objects, 5-6: change lights", 20);
	instructions->set_color(Ness::Color(1.0f, 1.0f, 1.0f, 0.5f));

	// create the corner logo
	Ness::SpritePtr corner_logo = scene->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
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
	Ness::Utils::make_logo_screen(scene, "../ness-engine/resources/gfx/logo.png");

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// update 3d sprite position
		light1->set_position(mouse.position());
		light2->set_position(mouse.position() + Ness::Point(200.0f, 0.0f));
		light3->set_position(mouse.position() - Ness::Point(200.0f, 0.0f));

		// keyboard control - toggle objects
		if (keyboard.key_state(SDLK_1))
		{
			sprite1->set_visible(true);
			sprite2->set_visible(false);
			sprite3->set_visible(false);
			sprite4->set_visible(false);
		}
		if (keyboard.key_state(SDLK_2))
		{
			sprite1->set_visible(false);
			sprite2->set_visible(true);
			sprite3->set_visible(false);
			sprite4->set_visible(false);
		}
		if (keyboard.key_state(SDLK_3))
		{
			sprite1->set_visible(false);
			sprite2->set_visible(false);
			sprite3->set_visible(true);
			sprite4->set_visible(false);
		}
		if (keyboard.key_state(SDLK_4))
		{
			sprite1->set_visible(false);
			sprite2->set_visible(false);
			sprite3->set_visible(false);
			sprite4->set_visible(true);
		}
		if (keyboard.key_state(SDLK_5))
		{
			light1->set_visible(true);
			light2->set_visible(false);
			light3->set_visible(false);
		}
		if (keyboard.key_state(SDLK_6))
		{
			light1->set_visible(false);
			light2->set_visible(true);
			light3->set_visible(true);
		}

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// cleanup
	renderer.remove_scene(scene);
	return 0;
}