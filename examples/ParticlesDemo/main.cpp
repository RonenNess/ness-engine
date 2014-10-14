/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

// Emit flame particles
class ParticlesEmitter : public Ness::ParticlesEmitter
{
	virtual Ness::ParticlePtr emit_particle(Ness::Renderer* renderer)
	{
		// create the particle to return
		Ness::ParticlePtr ret = ness_make_ptr<Ness::Particle>(renderer, "../ness-engine/resources/gfx/flame.png");

		// set random position and size, anchor to center
		ret->set_position(Ness::Point(-50.0f + rand() % 100, -50.0f + rand() % 100));
		ret->set_scale(0.5f + ((float)(rand() % 10) / 10.0f));
		ret->set_anchor(Ness::Point::HALF);

		// some flame particles will be additive blending, some will be normally blend
		// this creates better effect and gives "meat" to the flames
		if (rand() % 10 < 5)
		{
			ret->set_blend_mode(Ness::BLEND_MODE_ADD);
		}
		else
		{
			ret->set_blend_mode(Ness::BLEND_MODE_BLEND);
		}

		// add animators to particle
		ret->register_animator(ness_make_ptr<Ness::Animators::AnimatorFaderOut>(ret, true, 2.0f, 0.35f));
		ret->register_animator(ness_make_ptr<Ness::Animators::AnimatorFaderIn>(ret, 4.5f));
		ret->register_animator(ness_make_ptr<Ness::Animators::AnimatorScaler>(ret, Ness::Point(1.0f, 1.0f)));
		return ret;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Hello World!", Ness::Sizei(512, 512));

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create the particles node
	Ness::ParticlesNodePtr particles = scene->create_particles_node(Ness::Size(150, 150));
	
	// set particles emit settings
	Ness::SParticlesNodeEmitSettings EmitSettings;
	EmitSettings.particles_emitter = ness_make_ptr<ParticlesEmitter>();
	EmitSettings.emitting_interval = 0.045f;
	EmitSettings.max_particles_count = 100;
	EmitSettings.max_particles_emit = 3;
	EmitSettings.min_particles_emit = 1;
	EmitSettings.chance_to_emit = 85;
	particles->set_emit_settings(EmitSettings);

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(app);

	// create mouse handler
	Ness::Utils::Mouse mouse;
	EventsPoller.add_handler(mouse);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// set the particles system to the mouse position
		particles->set_position(mouse.position());

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();
	}

	// finish
	return 0;
}