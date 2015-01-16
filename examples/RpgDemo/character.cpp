#include "character.h"
#include "fireball.h"

Character::Character(Ness::LightNodePtr& lightNode, Ness::NodePtr& parent, const Ness::String& sprite)
{
	// create character sprite
	m_parent = parent;
	m_sprite = parent->create_sprite(sprite);
	m_sprite->set_scale(2.5f);
	m_sprite->set_anchor(Ness::Point(0.5f, 1.0f));
	m_sprite->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_sprite->set_source_from_sprite_sheet(Ness::Pointi(0, 0), Ness::Pointi(4, 4), true);
	m_last_direction = DIRECTION_DOWN;

	// create the light
	m_light_node = lightNode;
	m_light = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::WHITE * 0.75f);
	m_light->set_scale(Ness::Point(2.0f, 1.75f));

	// shooting cooldown
	m_fire_cooldown = 0.0f;

	// to set direction and create animator
	m_is_walking = false;
	m_last_direction = DIRECTION_DOWN;
	m_animator = ness_make_ptr<Ness::Animators::AnimatorSprite>(m_sprite, Ness::Pointi(4, 4), m_last_direction * 4, 4, 10.0f, Ness::Animators::SPRITE_ANIM_END_REPEAT);
	m_animator->pause_animation(true);
	m_sprite->renderer()->register_animator(m_animator);
}

void Character::do_animation(Ness::Renderer* renderer)
{
	// do walking movement
	if (m_is_walking)
	{
		float factor = 250.0f;
		switch (m_last_direction)
		{
		case DIRECTION_DOWN:
			m_sprite->set_position(m_sprite->get_position() + Ness::Point(0, renderer->time_factor() * factor));
			break;
		case DIRECTION_UP:
			m_sprite->set_position(m_sprite->get_position() - Ness::Point(0, renderer->time_factor() * factor));
			break;
		case DIRECTION_LEFT:
			m_sprite->set_position(m_sprite->get_position() - Ness::Point(renderer->time_factor() * factor, 0));
			break;
		case DIRECTION_RIGHT:
			m_sprite->set_position(m_sprite->get_position() + Ness::Point(renderer->time_factor() * factor, 0));
			break;
		}
	}

	// fix zindex based on position
	m_sprite->set_zindex(m_sprite->get_position().y);

	// make light follow player
	m_light->set_position(get_position() - Ness::Point(0.0f, m_sprite->get_absolute_size().y * 0.25f));
	m_light->set_zindex(m_sprite->get_zindex());
}

void Character::stop()
{
	m_animator->pause_animation(true);
	m_is_walking = false;
}

void Character::shoot()
{
	if (m_sprite->renderer()->get_total_time_elapse() - m_fire_cooldown < 0.5f)
		return;
	m_fire_cooldown = m_sprite->renderer()->get_total_time_elapse();
	Ness::Animators::AnimatorPtr new_fireball = ness_make_ptr<Fireball>(m_light_node, m_parent, get_position(), m_last_direction);
	m_parent->renderer()->register_animator(new_fireball);
}

void Character::walk(EDirection direction)
{
	m_is_walking = true;

	// if walking to the same direction nothing to do
	if (m_last_direction == direction)
	{
		m_animator->pause_animation(false);
		return;
	}

	// set direction
	m_last_direction = direction;

	// set animation to walking at given direction
	m_animator->reset(m_last_direction * 4, 4, 10.0f);
}