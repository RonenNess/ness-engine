#include "player.h"

Player::Player(Ness::NodePtr parent, Ness::LightNodePtr lightNode)
{
	// create the player sprite
	m_sprite = parent->create_sprite("spaceman.png");
	m_sprite->set_anchor(Ness::Point(0.5f, 0.5f));
	m_sprite->set_size(Ness::Sizei(64, 64));
	m_sprite->set_position(Ness::Point(400.0f, 510.0f));
	m_sprite->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// create the player light
	m_light = lightNode->create_light("../ness-engine/resources/gfx/light_beam_detailed.jpg", Ness::Color::WHITE);
	m_round_light = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE);
	m_round_light->set_scale(0.35f);
	m_round_light->set_color(Ness::Color::GREY);
	
	// set default action and direction
	m_action = PLAYER_STANDING;
	m_direction = DIRECTION_RIGHT;

	// default light direction is forward
	m_light_direction = 0;
}

void Player::do_events()
{

	// set light position and anchor based on direction
	if (m_direction == DIRECTION_RIGHT)
	{
		m_light->set_anchor(Ness::Point(0.015f, 0.5f));
		m_light->set_rotation(m_light_direction);
	}
	else
	{
		m_light->set_anchor(Ness::Point(1.0f - 0.015f, 0.5f));
		m_light->set_rotation(-m_light_direction);
	}
	m_light->set_position(m_sprite->get_position());
	m_round_light->set_position(m_sprite->get_position());
	m_light->set_scale(m_sprite->get_scale());

	// moving the player
	Ness::Point pos = m_sprite->get_position();
	if (m_action == PLAYER_WALKING)
	{
		pos.x += m_sprite->renderer()->time_factor() * 100.0f * (m_direction == DIRECTION_LEFT ? -1.0f : 1.0f);
		if (pos.x < 0)
			pos.x = 0;
		if (pos.x > m_sprite->renderer()->get_screen_size().x)
			pos.x = (float)m_sprite->renderer()->get_screen_size().x;
	}
	m_sprite->set_position(pos);
}

void Player::aim_light_up()
{
	m_light_direction -= m_sprite->renderer()->time_factor() * 100.0f;
	put_light_in_range();
}

void Player::aim_light_down()
{
	m_light_direction +=  m_sprite->renderer()->time_factor() * 100.0f;
	put_light_in_range();
}

void Player::put_light_in_range()
{
	if (m_light_direction < -70) m_light_direction = -70;
	if (m_light_direction > 70)	m_light_direction = 70;
}

void Player::stand()
{
	change_action(PLAYER_STANDING);
}

void Player::walk_left()
{
	m_direction = DIRECTION_LEFT;
	if (m_action != PLAYER_WALKING)
		change_action(PLAYER_WALKING);
	m_sprite->set_scale(Ness::Size(-1.0f, 1.0f));
}

void Player::walk_right()
{
	m_direction = DIRECTION_RIGHT;
	if (m_action != PLAYER_WALKING)
		change_action(PLAYER_WALKING);
	m_sprite->set_scale(Ness::Size(1.0f, 1.0f));
}

void Player::change_action(EPlayerAction newAction)
{
	// how many total animation steps we got in the spaceman texturesheet
	static const Ness::Sizei SpacemanTotalSteps(10, 2);

	// remove previous animator
	if (m_animation)
	{
		m_animation->remove_animation();
		m_animation.reset();
	}

	// set animator / animation step based on action
	switch (newAction)
	{
		case PLAYER_STANDING:
			m_sprite->set_source_from_sprite_sheet(Ness::Pointi(0,1), SpacemanTotalSteps);
			break;

		case PLAYER_WALKING:
			m_animation = ness_make_ptr<Ness::Animators::AnimatorSprite>(m_sprite, SpacemanTotalSteps, 0, 10, 10.0f, Ness::Animators::SPRITE_ANIM_END_REPEAT);
			m_sprite->renderer()->register_animator(m_animation);
			break;
	}

	// set current action
	m_action = newAction;
}