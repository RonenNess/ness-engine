/*
* NessEngine SpaceFighter demo - this is a simple 2 players game with spaceship and meteroids.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>
#include "player.h"
#include "meteor.h"
#include <memory>
#include <time.h>

// is the program still running
bool g_running = true;

// resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// callback to handle exit events
void HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_QUIT)
		g_running = false;
}

// function to remove all meteors that are off-screen from the list of meteors
bool remove_meteors_off_screen(const std::auto_ptr<Meteor>& meteor)
{
	if (meteor->get_position().x < -meteor->get_size() || meteor->get_position().y < -meteor->get_size())
		return true;
	if (meteor->get_position().x > SCREEN_WIDTH + meteor->get_size() || meteor->get_position().y > SCREEN_HEIGHT + meteor->get_size())
		return true;
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand ((unsigned int)time(NULL));

	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("SpaceFighter demo!", Ness::Sizei(SCREEN_WIDTH, SCREEN_HEIGHT));

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create the background
	Ness::SpritePtr background = scene->create_sprite("background.jpg");

	// create the node for players and meteors.
	// note: meteors node is created first, so players and their effects will always be above meteors
	Ness::NodePtr meteorsNode = scene->create_node();
	Ness::NodePtr playersNode = scene->create_node();

	// creating player 1 (defined in player.h)
	Player player1(playersNode, "player1.png", Ness::Color(1.0f, 0.2f, 0.0f, 1.0f));
	player1.set_position(renderer.get_screen_center() + Ness::Point(0, 100));
	player1.set_rotation(90);

	// creating player 2
	Player player2(playersNode, "player2.png", Ness::Color(0.0f, 1.0f, 0.4f, 1.0f));
	player2.set_position(renderer.get_screen_center() + Ness::Point(0, -100));
	player2.set_rotation(-90);

	// random meteors
	std::list<std::auto_ptr<Meteor> > meteors;
	float timeUntilNextMeteor = (float)(rand() % 20) / 10.0f;

	// create the fps show
	Ness::TextPtr fpsShow = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "fps", 20);
	fpsShow->set_size(Ness::Size(100, 100));
	fpsShow->set_static(true);

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

		// spawn meteors randomly
		timeUntilNextMeteor -= renderer.time_factor();
		if (timeUntilNextMeteor <= 0.0f)
		{
			int MeteorDirection = rand() % 360;
			float MeteorSize = 50.0f + (rand() % 100);
			float MeteorSpeed = 0.5f + (float)(rand() % 10) / 15.0f;
			Ness::Point MeteorSpeedVector = Ness::Point::from_angle(MeteorDirection, MeteorSpeed);
			Meteor* meteor = new Meteor(meteorsNode, MeteorSpeedVector, MeteorSize);
			Ness::Point position;
			if (rand() % 10 < 5)
			{
				if (MeteorSpeedVector.y > 0)
					position.y = -MeteorSize;
				else
					position.y = renderer.get_screen_size().y + MeteorSize;
				position.x = (float)(rand() % renderer.get_screen_size().x);
			}
			else
			{
				if (MeteorSpeedVector.x > 0)
					position.x = -MeteorSize;
				else
					position.x = renderer.get_screen_size().x + MeteorSize;
				position.y = (float)(rand() % renderer.get_screen_size().y);
			}
			meteor->set_position(position);

			meteors.push_back(std::auto_ptr<Meteor>(meteor));
			timeUntilNextMeteor = (float)(rand() % 100) / 10.0f;
		}

		// render the scene
		renderer.start_frame();

		// do animatins
		renderer.do_animations();

		// do players events
		player1.do_events();
		player2.do_events();

		// do collisions
		player1.do_collisions(meteors, player2);
		player2.do_collisions(meteors, player1);

		// do all meteors events
		for (auto meteor = meteors.begin(); meteor != meteors.end(); ++meteor)
		{
			// move meteor
			(*meteor)->do_events();
		}
		// remove all meteors that gone out of screen
		meteors.remove_if(remove_meteors_off_screen);

		// KEYBOARD CONTROL:

		// player1 controls
		if (keyboard.ket_state(SDLK_UP))
			player1.fly_forward();
		if (keyboard.ket_state(SDLK_DOWN))
			player1.fly_backwards();
		if (keyboard.ket_state(SDLK_LEFT))
			player1.turn_left();
		if (keyboard.ket_state(SDLK_RIGHT))
			player1.turn_right();
		if (keyboard.ket_state(SDLK_RETURN))
			player1.fire();

		// player2 controls
		if (keyboard.ket_state(SDLK_w))
			player2.fly_forward();
		if (keyboard.ket_state(SDLK_s))
			player2.fly_backwards();
		if (keyboard.ket_state(SDLK_a))
			player2.turn_left();
		if (keyboard.ket_state(SDLK_d))
			player2.turn_right();
		if (keyboard.ket_state(SDLK_SPACE))
			player2.fire();

		// exit
		if (keyboard.ket_state(SDLK_ESCAPE))
		{
			g_running = false;
		}

		// render and end the scene
		scene->render();
		renderer.end_frame();

		// update fps show
		std::string FpsShow = std::string("fps ") + (renderer.get_flags() & Ness::RENDERER_FLAG_VSYNC ? "(vsync): " : ": ");
		fpsShow->change_text(FpsShow + std::to_string((long long)renderer.fps()));
	}

	// cleanup. 
	// note: the 'remove' lines are not mandatory, they are just to illustrate how to remove an entity from the scene.
	scene->remove(playersNode);
	scene->remove(meteorsNode);
	scene->remove(background);
	return 0;
}