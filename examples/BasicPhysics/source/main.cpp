// this ness-engine example shows how to integrate ness-engine with Box2D physics.
// Author: Ronen Ness
// Since: 20/08/2014

#include <NessEngine.h>
#include <Box2D/Box2D.h>
#include "box2d_converter.h"
#include "debug_draw.h"

// advance the physical simulator steps (must be called every frame to do physics)
void physics_step(b2World& world, float msdelta)
{
	static Uint32 time = Ness::get_ticks();
	static Uint32 lasttime = time;

    time = Ness::get_ticks();
    world.Step((time - lasttime) / 1000.0f, 10, 10);
    lasttime = time; // lasttime is a member variable that holds the ticks since the last loop
}

int _tmain(int argc, _TCHAR* argv[])
{
	// create the renderer
	Ness::init();
    Ness::Renderer renderer("new project", Ness::Sizei(800,600), false);

    // create the scene and a node
    Ness::ScenePtr scene = renderer.create_scene();
    Ness::NodePtr node = scene->create_node();

	// create background
	Ness::SpritePtr back = node->create_sprite("../ness-engine/resources/gfx/forest_background.jpg");
	back->set_size(renderer.get_screen_size());
	
	// Define the gravity vector. and create the physical world simulator
	b2Vec2 gravity(0.0f, 9.8f);
	b2World world(gravity);

	// create debug draw for box2d
	Box2dDebugDraw physics_debug(&renderer, Ness::CameraPtr());
	world.SetDebugDraw(&physics_debug);
	physics_debug.SetFlags( b2Draw::e_shapeBit );

	// first, we create the Box2D body for the crab

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, -40.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2CircleShape crabShape;
	crabShape.m_radius = 0.5f;

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &crabShape;
	fixtureDef.density = 2.5f;
	fixtureDef.restitution = 0.5f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// now we'll create a ness-engine sprite and connect it with the physical body

	Ness::SpritePtr player = node->create_sprite("../ness-engine/resources/gfx/crab.png");
	player->set_anchor(Ness::Point::HALF);
	player->set_blend_mode(Ness::BLEND_MODE_BLEND);
	player->set_scale(0.25f);

	// and we use our new connector class to connect the two:
	NessBoxConnector playerBodyConnector(player, body);
	playerBodyConnector.set_position(Ness::Pointi(renderer.get_screen_center().x, 0));
	renderer.__register_animator_unsafe(&playerBodyConnector);

	// now we create the physical body for the floor

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 15.5f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(350.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

    // event handlers
    Ness::Utils::EventsPoller EventsPoller;
    Ness::Utils::ApplicationEvents app;
    EventsPoller.add_handler(app);
    Ness::Utils::Mouse mouse;
    EventsPoller.add_handler(mouse);
	Ness::Utils::Keyboard keyboard;
    EventsPoller.add_handler(keyboard);

	// create the corner logo
	Ness::SpritePtr corner_logo = scene->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
	corner_logo->set_position(renderer.get_screen_size());

    // main loop
	bool keep_running = true;
    while( !app.got_quit() && keep_running )
    {
		// poll all events
        EventsPoller.poll_events();

		if (keyboard.key_state(SDLK_UP))
		{
			body->ApplyForce( b2Vec2(0,-50), body->GetWorldCenter(), true);
		}
		if (keyboard.key_state(SDLK_DOWN))
		{
			body->ApplyForce( b2Vec2(0,50), body->GetWorldCenter(), true);
		}
		if (keyboard.key_state(SDLK_LEFT))
		{
			body->ApplyForce( b2Vec2(-15,0), body->GetWorldCenter(), true);
		}
		if (keyboard.key_state(SDLK_RIGHT))
		{
			body->ApplyForce( b2Vec2(15,0), body->GetWorldCenter(), true);
		}

		// when mouse click we generate a small evil crab
		static bool mouse_was_down = false;
		if (mouse.is_down(Ness::MOUSE_LEFT))
		{
			if (!mouse_was_down)
			{
				b2BodyDef bodyDef;
				bodyDef.type = b2_dynamicBody;
				bodyDef.position = NessToBox2d::point(mouse.position());
				b2Body* body = world.CreateBody(&bodyDef);
				b2CircleShape Shape;
				Shape.m_radius = 0.35f;
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &Shape;
				fixtureDef.density = 2.5f;
				fixtureDef.restitution = 0.5f;
				body->CreateFixture(&fixtureDef);
				Ness::SpritePtr new_crab = node->create_sprite("../ness-engine/resources/gfx/crab.png");
				new_crab->set_anchor(Ness::Point::HALF);
				new_crab->set_blend_mode(Ness::BLEND_MODE_BLEND);
				new_crab->set_scale(0.125f);
				new_crab->set_color(Ness::Color::BLUE);
				renderer.__register_animator_unsafe(new NessBoxConnector(new_crab, body));
			}
			mouse_was_down = true;
		}
		else
		{
			mouse_was_down = false;
		}

        // render
        renderer.start_frame();
        scene->render();
		physics_step(world, renderer.time_factor() * 1000.0f);
		world.DrawDebugData();
        renderer.end_frame();
    }
}