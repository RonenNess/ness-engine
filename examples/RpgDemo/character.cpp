#include "character.h"

Character::Character(Ness::NodePtr& parent, const Ness::String& sprite)
{
	m_parent = parent;
	m_sprite = parent->create_sprite(sprite);
	m_sprite->set_scale(2.5f);
	m_sprite->set_anchor(Ness::Point(0.5f, 1.0f));
	m_sprite->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_sprite->set_source_from_sprite_sheet(Ness::Pointi(0, 0), Ness::Pointi(4, 4), true);
	m_last_direction = DIRECTION_DOWN;

	// to set direction and create animator
	m_is_walking = false;
	m_last_direction = DIRECTION_UP;
	walk(DIRECTION_DOWN);
	stop();
}

void Character::stop()
{
	m_animator->pause_animation(true);
	m_is_walking = false;
}

void Character::shoot()
{
	Ness::SpritePtr fireball = m_parent->create_sprite("gfx/fireball.png");
	fireball->set_position(get_position() - Ness::Point(0, 50));
	fireball->set_zindex(m_sprite->get_zindex());
	fireball->set_anchor(Ness::Point::HALF);
	fireball->set_blend_mode(Ness::BLEND_MODE_BLEND);
	fireball->set_source_from_sprite_sheet(Ness::Pointi(0, m_last_direction), Ness::Pointi(3, 4), true);
}

void Character::walk(EDirection direction)
{
	// if walking to the same direction nothing to do
	if (m_last_direction == direction)
	{
		m_animator->pause_animation(false);
		return;
	}

	// set direction
	m_is_walking = true;
	m_last_direction = direction;

	// set animation to walking at given direction
	if (m_animator)
		m_animator->remove_animation();
	m_animator = ness_make_ptr<Ness::Animators::AnimatorSprite>(m_sprite, Ness::Pointi(4, 4), m_last_direction * 4, 3, 10.0f, Ness::Animators::SPRITE_ANIM_END_REPEAT);
	m_sprite->renderer()->register_animator(m_animator);
}