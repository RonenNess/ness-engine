#include <NessEngine.h>
#include <algorithm>
#define ANIMATION_SPEED 10.0f
#define MOVEMENT_SPEED 500.0f
float g_score = 0;
bool remove_meteors_off_screen(const Ness::SpritePtr& meteor)
{
	if (meteor->get_position().y < meteor->renderer()->get_screen_size().y + 100)
		return false;
	g_score += meteor->get_scale().x;
	meteor->parent()->remove(meteor);
	return true;
}
int _tmain(int argc, char* argv[])
{
    Ness::init();
	Ness::Renderer renderer("Spaceship", Ness::Sizei(800,600), Ness::DEFAULT_WINDOW_FLAGS, Ness::DEFAULT_RENDERER_FLAGS | Ness::RENDERER_FLAG_VSYNC);
    Ness::ScenePtr scene = renderer.create_scene();
	Ness::NodePtr backNode = scene->create_node();
	for (int i = 0; i < 2; i++) {
		Ness::SpritePtr back = backNode->create_sprite("../ness-engine/resources/gfx/space.jpg");
		back->set_size(renderer.get_screen_size());
		back->set_position(Ness::Pointi(0, -renderer.get_screen_size().y * i));
	}
	float backgroundAnim = 0.0f;
	Ness::Sprite::Defaults.anchor = Ness::Point::HALF;
	Ness::Sprite::Defaults.alpha_channels = true;
	Ness::NodePtr meteorsNode = scene->create_node();
    Ness::SpritePtr player = scene->create_sprite("../ness-engine/resources/gfx/spaceship.png");
	player->set_scale(0.75f);
	player->set_position((Ness::Point)(renderer.get_screen_size()) * Ness::Point(0.5f, 0.9f));
	Ness::Containers::List<Ness::SpritePtr> meteors;
	Ness::Animators::AnimatorSpritePtr playerAnim = ness_make_ptr<Ness::Animators::AnimatorSprite>(player, Ness::Sizei(3, 3), Ness::Sizei(0, 0), 3, ANIMATION_SPEED, Ness::Animators::SPRITE_ANIM_END_REPEAT);
	renderer.register_animator(playerAnim);
    Ness::Utils::EventsPoller EventsPoller;
    Ness::Utils::ApplicationEvents app;
	Ness::Utils::Keyboard keyboard;
    EventsPoller.add_handler(app);
	EventsPoller.add_handler(keyboard);
	bool is_player_alive = true;
	Ness::TextPtr scoreShow = scene->create_text("../ness-engine/resources/fonts/courier.ttf", "score", 24);
    while( !app.got_quit() ) {
        EventsPoller.poll_events();
		if (rand() % 700 < 5 + std::min(g_score * 0.05f, 22.5f)) {
			Ness::SpritePtr meteor = meteorsNode->create_sprite("../ness-engine/resources/gfx/meteor.png");
			meteor->set_position(Ness::Point((float)(rand() % renderer.get_screen_size().x), -100.0f));
			meteor->set_scale(0.75f + ((rand() % 30) / 10.0f));
			meteors.push_back(meteor);
		}
		for (auto meteor = meteors.begin(); meteor != meteors.end(); ++meteor) {
			float speed = renderer.time_factor() * 100.0f * (*meteor)->get_scale().x;
			(*meteor)->set_position((*meteor)->get_position() + Ness::Point(0, speed));
			(*meteor)->set_rotation((*meteor)->get_rotation() + speed);
			if (is_player_alive && (*meteor)->get_position().distance(player->get_position()) < player->get_absolute_size().x * 0.45f + (*meteor)->get_absolute_size().x * 0.35f) {
				is_player_alive = false;
				Ness::SpritePtr exp = scene->create_sprite("../ness-engine/resources/gfx/explosion.png");
				exp->set_position(player->get_position());
				exp->set_size(player->get_size());
				exp->set_scale(player->get_scale() * 1.5f);
				renderer.register_animator(ness_make_ptr<Ness::Animators::AnimatorSprite>(exp, Ness::Sizei(5, 5), 0, 25, 15.0f, Ness::Animators::SPRITE_ANIM_END_REMOVE_SPRITE));
				renderer.register_animator(ness_make_ptr<Ness::Animators::AnimatorFaderOut>(player, true, 1.0f));
			}
		}
		meteors.remove_if(remove_meteors_off_screen);
		if (is_player_alive) {
			if (keyboard.key_state(SDLK_LEFT)) {
				if (playerAnim->get_starting_step_point() != Ness::Sizei(0, 1))
					playerAnim->reset(Ness::Sizei(0, 1), 3, ANIMATION_SPEED);
				if (player->get_position().x > player->get_size().x * 0.5f)
					player->set_position(player->get_position() - Ness::Point(renderer.time_factor() * MOVEMENT_SPEED, 0));
			}
			else if (keyboard.key_state(SDLK_RIGHT)) {
				if (playerAnim->get_starting_step_point() != Ness::Sizei(0, 2))
					playerAnim->reset(Ness::Sizei(0, 2), 3, ANIMATION_SPEED);
				if (player->get_position().x < renderer.get_screen_size().x - player->get_size().x * 0.5f)
					player->set_position(player->get_position() + Ness::Point(renderer.time_factor() * MOVEMENT_SPEED, 0));
			}
			else {
				if (playerAnim->get_starting_step_point() != Ness::Sizei(0, 0))
					playerAnim->reset(Ness::Sizei(0, 0), 3, ANIMATION_SPEED);
			}
			if (keyboard.key_state(SDLK_UP)) {
				if (player->get_position().y > player->get_size().x * 0.5f)
					player->set_position(player->get_position() - Ness::Point(0, renderer.time_factor() * MOVEMENT_SPEED));
			}
			if (keyboard.key_state(SDLK_DOWN)) {
				if (player->get_position().y < renderer.get_screen_size().y)
					player->set_position(player->get_position() + Ness::Point(0, renderer.time_factor() * MOVEMENT_SPEED));
			}
			scoreShow->change_text("Score: " + ness_int_to_string(g_score));
		}
		backNode->set_position(Ness::Point(0, backgroundAnim));
		backgroundAnim += renderer.time_factor() * 150.0f;
		if (backgroundAnim >= renderer.get_screen_size().y)
			backgroundAnim -= renderer.get_screen_size().y;
        renderer.start_frame();
        scene->render();
        renderer.end_frame();
		g_score += renderer.time_factor();
    }
	return 0;
}