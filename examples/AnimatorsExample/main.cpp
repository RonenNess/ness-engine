/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

int _tmain(int argc, char* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Hello World!", Ness::Sizei(512, 512));

	{

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create sprite with fader animator
	Ness::SpritePtr sprite1 = scene->create_sprite("hello_world.png");
	sprite1->set_blend_mode(Ness::BLEND_MODE_BLEND);
	sprite1->set_size(Ness::Size(250, 250));
	Ness::Animators::AnimatorFaderOutPtr anim = ness_make_ptr<Ness::Animators::AnimatorFaderOut>(sprite1, true, 0.5f, 1.0f);
	renderer.register_animator(anim);

	// create sprite with color shifter animator
	Ness::SpritePtr sprite2 = scene->create_sprite("hello_world.png");
	sprite2->set_size(Ness::Size(250, 250));
	sprite2->set_position(Ness::Point(255, 0));
	Ness::Animators::AnimatorColorShifterPtr anim2 = ness_make_ptr<Ness::Animators::AnimatorColorShifter>(sprite2, Ness::Color::RED, Ness::Color::GREEN, 2.0f, 1.0f);
	renderer.register_animator(anim2);

	// create sprite with rotation animator
	Ness::SpritePtr sprite3 = scene->create_sprite("hello_world.png");
	sprite3->set_size(Ness::Size(250, 250));
	sprite3->set_anchor(Ness::Point::HALF);
	sprite3->set_position(Ness::Point(125, 255 + 125));
	Ness::Animators::AnimatorRotatorPtr anim3 = ness_make_ptr<Ness::Animators::AnimatorRotator>(sprite3, 36.0f, 10.0f);
	renderer.register_animator(anim3);

	// create sprite with rotation animator
	Ness::SpritePtr sprite4 = scene->create_sprite("hello_world.png");
	sprite4->set_size(Ness::Size(250, 250));
	sprite4->set_position(Ness::Point(255, 255));
	Ness::Animators::AnimatorScalerPtr anim4 = ness_make_ptr<Ness::Animators::AnimatorScaler>(sprite4, Ness::Point(-0.2f, -0.2f), 10.0f, 1.0f);
	renderer.register_animator(anim4);

	// create animated sprite example
	Ness::AnimatedSpritePtr AnimSprite = scene->create_animated_sprite("hello_world.png");
	AnimSprite->set_size(Ness::Size(150, 150));
	AnimSprite->set_anchor(Ness::Point::HALF);
	AnimSprite->set_position(Ness::Point(252, 252));
	AnimSprite->register_animator(ness_make_ptr<Ness::Animators::AnimatorColorShifter>(AnimSprite, Ness::Color::BLACK, Ness::Color::WHITE, 5.0f, 1.0f));

	// create the corner logo
	Ness::SpritePtr corner_logo = scene->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
	corner_logo->set_position(renderer.get_screen_size());

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(app);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	}

	// finish
	return 0;
}