// this ness-engine example shows how to integrate ness-engine with Box2D physics.
// Author: Ronen Ness
// Since: 20/08/2014

#include <NessEngine.h>
#include <Box2D/Box2D.h>
#include "box2d_converter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// create the renderer
	Ness::init();
    Ness::Renderer renderer("new project", Ness::Sizei(800,600), false);

    // create the scene and a node
    Ness::ScenePtr scene = renderer.create_scene();
    Ness::NodePtr node = scene->create_node();
	
	// Define the gravity vector. and create the physical world simulator
	b2Vec2 gravity(0.0f, 9.8f);
	b2World world(gravity);

	// first, we create the Box2D body (as a dynamic box):

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, -40.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// now we'll create a ness-engine sprite

	Ness::SpritePtr player = node->create_sprite("../ness-engine/resources/gfx/crab.png");
	player->set_anchor(Ness::Point::HALF);
	player->set_blend_mode(Ness::BLEND_MODE_BLEND);
	player->set_scale(0.25f);

	// and we use our new connector class to connect the two:
	NessBoxConnector playerBodyConnector(player, body);
	playerBodyConnector.set_position(Ness::Pointi(renderer.get_screen_center().x, 0));

	// two consts for the physical simulator
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;

    // event handlers
    Ness::Utils::EventsPoller EventsPoller;
    Ness::Utils::ApplicationEvents app;
    EventsPoller.add_handler(app);
    Ness::Utils::Mouse mouse;
    EventsPoller.add_handler(mouse);
	Ness::Utils::Keyboard keyboard;
    EventsPoller.add_handler(keyboard);

    // main loop
	bool keep_running = true;
    while( !app.got_quit() && keep_running )
    {
		// poll all events
        EventsPoller.poll_events();

		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(1 / 10000.0f, velocityIterations, positionIterations);

		if (keyboard.key_state(SDLK_UP))
		{
			body->ApplyForce( b2Vec2(0,-75), body->GetWorldCenter(), true);
		}

		playerBodyConnector.update();

        // render
        renderer.start_frame();
        scene->render();
        renderer.end_frame();
    }
}