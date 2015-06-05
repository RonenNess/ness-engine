
// NOTE!!! THIS IS THE LONG, PRETTIER VERSION OF THIS EXAMPLE.
// TO SEE THE 100 LINES ONLY CODE, COMPILE '100_lines_of_code.cpp' INSTEAD!

#include <algorithm>
#include <NessEngine.h>

// spaceship animation speed
#define ANIMATION_SPEED 10.0f

// movement speed
#define MOVEMENT_SPEED 500.0f

// count score
float g_score = 0;

// remove meteors that are off-screen
bool remove_meteors_off_screen(const Ness::SpritePtr& meteor)
{
	if (meteor->get_position().y > meteor->renderer()->get_screen_size().y + 100)
	{
		g_score += meteor->get_scale().x;
		meteor->parent()->remove(meteor);
		return true;
	}
	return false;
}

#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create renderer
    Ness::init();
    Ness::Renderer renderer("Spaceship", Ness::Sizei(800,600));

	// create the scene
    Ness::ScenePtr scene = renderer.create_scene();

	// create background node
	Ness::NodePtr backNode = scene->create_node();
	Ness::SpritePtr back = backNode->create_sprite("../ness-engine/resources/gfx/space.jpg");
	back->set_size(renderer.get_screen_size());
	back = backNode->create_sprite("../ness-engine/resources/gfx/space.jpg");
	back->set_size(renderer.get_screen_size());
	back->set_position(Ness::Pointi(0, -renderer.get_screen_size().y));
	float backgroundAnim = 0.0f;

	// create node for meteors (below the player)
	Ness::NodePtr meteorsNode = scene->create_node();

	// create the player
    Ness::SpritePtr player = scene->create_sprite("../ness-engine/resources/gfx/spaceship.png");
	player->set_blend_mode(Ness::BLEND_MODE_BLEND);
	player->set_anchor(Ness::Point::HALF);
	player->set_scale(0.75f);
	player->set_position((Ness::Point)(renderer.get_screen_size()) * Ness::Point(0.5f, 0.9f));

	// create meteors list
	Ness::Containers::List<Ness::SpritePtr> meteors;

	// create player animator
	Ness::Animators::AnimatorSpritePtr playerAnim = ness_make_ptr<Ness::Animators::AnimatorSprite>
		(player, Ness::Sizei(3, 3), Ness::Sizei(0, 0), 3, ANIMATION_SPEED, Ness::Animators::SPRITE_ANIM_END_REPEAT);
	renderer.register_animator(playerAnim);

	// create the corner logo
	Ness::SpritePtr corner_logo = scene->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
	corner_logo->set_static(true);
	corner_logo->set_position(renderer.get_screen_size());

	// create events handlers
    Ness::Utils::EventsPoller EventsPoller;
    Ness::Utils::ApplicationEvents app;
	Ness::Utils::Keyboard keyboard;
    EventsPoller.add_handler(app);
	EventsPoller.add_handler(keyboard);

	// is the player still alive?
	bool is_player_alive = true;

	// show score
	Ness::TextPtr scoreShow = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "score", 24);

	// time until next meteor spawn
	float TimeUntilNextSpawn = 1.35f;

	// show logo screen
	Ness::Utils::make_logo_screen(scene, "../ness-engine/resources/gfx/logo.png");

    while( !app.got_quit() )
    {
		// poll events
        EventsPoller.poll_events();

		// spawn random meteors
		if (TimeUntilNextSpawn <= 0.0f)
		{
			Ness::SpritePtr meteor = meteorsNode->create_sprite("../ness-engine/resources/gfx/meteor.png");
			meteor->set_anchor(Ness::Point::HALF);
			meteor->set_position(Ness::Point((float)(rand() % renderer.get_screen_size().x), -100.0f));
			meteor->set_scale(0.75f + ((rand() % 30) / 10.0f));
			meteor->set_blend_mode(Ness::BLEND_MODE_BLEND);
			meteors.push_back(meteor);
			TimeUntilNextSpawn = 0.5f + (rand() % 10) / 5.5f - std::min(g_score / 1000.0f, 0.6f);
		}
		TimeUntilNextSpawn -= renderer.time_factor();

		// move meteors
		for (auto meteor = meteors.begin(); meteor != meteors.end(); ++meteor)
		{
			// move the meteors
			float speed = renderer.time_factor() * 100.0f * (*meteor)->get_scale().x;
			(*meteor)->set_position((*meteor)->get_position() + Ness::Point(0, speed));
			(*meteor)->set_rotation((*meteor)->get_rotation() + speed);

			// check if collide with player
			if (is_player_alive && (*meteor)->get_position().distance(player->get_position()) < player->get_absolute_size().x * 0.45f + (*meteor)->get_absolute_size().x * 0.35f)
			{
				// create explosion and set player to dead
				is_player_alive = false;
				Ness::SpritePtr exp = scene->create_sprite("../ness-engine/resources/gfx/explosion.png");
				exp->set_anchor(player->get_anchor());
				exp->set_size(player->get_size());
				exp->set_scale(player->get_scale() * 1.5f);
				exp->set_position(player->get_position());
				exp->set_blend_mode(Ness::BLEND_MODE_BLEND);
				renderer.register_animator(ness_make_ptr<Ness::Animators::AnimatorSprite>(exp, Ness::Sizei(5, 5), 0, 25, 15.0f, Ness::Animators::SPRITE_ANIM_END_REMOVE_SPRITE));
				renderer.register_animator(ness_make_ptr<Ness::Animators::AnimatorFaderOut>(player, true, 1.0f));
			}
		}
		meteors.remove_if(remove_meteors_off_screen);

		// move player
		if (is_player_alive)
		{
			if (keyboard.key_state(SDLK_LEFT))
			{
				if (playerAnim->get_starting_step_point() != Ness::Sizei(0, 1))
					playerAnim->reset(Ness::Sizei(0, 1), 3, ANIMATION_SPEED);
				if (player->get_position().x > player->get_size().x * 0.5f)
					player->set_position(player->get_position() - Ness::Point(renderer.time_factor() * MOVEMENT_SPEED, 0));
			}
			else if (keyboard.key_state(SDLK_RIGHT))
			{
				if (playerAnim->get_starting_step_point() != Ness::Sizei(0, 2))
					playerAnim->reset(Ness::Sizei(0, 2), 3, ANIMATION_SPEED);
				if (player->get_position().x < renderer.get_screen_size().x - player->get_size().x * 0.5f)
					player->set_position(player->get_position() + Ness::Point(renderer.time_factor() * MOVEMENT_SPEED, 0));
			}
			else
			{
				if (playerAnim->get_starting_step_point() != Ness::Sizei(0, 0))
					playerAnim->reset(Ness::Sizei(0, 0), 3, ANIMATION_SPEED);
			}
			if (keyboard.key_state(SDLK_UP))
			{
				if (player->get_position().y > player->get_size().x * 0.5f)
					player->set_position(player->get_position() - Ness::Point(0, renderer.time_factor() * MOVEMENT_SPEED));
			}
			if (keyboard.key_state(SDLK_DOWN))
			{
				if (player->get_position().y < renderer.get_screen_size().y)
					player->set_position(player->get_position() + Ness::Point(0, renderer.time_factor() * MOVEMENT_SPEED));
			}
		}

		// animate background
		backNode->set_position(Ness::Point(0, backgroundAnim));
		backgroundAnim += renderer.time_factor() * 150.0f;
		if (backgroundAnim >= renderer.get_screen_size().y)
			backgroundAnim -= renderer.get_screen_size().y;

		// render everything
        renderer.start_frame();
        scene->render();
        renderer.end_frame();

		// update score
		if (is_player_alive)
		{
			scoreShow->change_text("Score: " + ness_int_to_string(g_score));
			g_score += renderer.time_factor();
		}
    }
}