#include "fireball.h"
#include "character.h"

#define FIREBALL_ANIM_STEPS 3
#define FIREBALL_DIRECTIONS_COUNT 4

Fireball::Fireball(Ness::LightNodePtr& lightNode, Ness::NodePtr& parent, const Ness::Point& position, const int direction)
{
	// create sprite and animator
	m_direction = direction;
	m_parent = parent;
	m_sprite = parent->create_animated_sprite("gfx/fireball.png");
	m_sprite->set_anchor(Ness::Point(0.5f, 0.5f));
	m_sprite->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_sprite->set_position(position - Ness::Point(0, 50));
	m_sprite->set_source_from_sprite_sheet(Ness::Pointi(0, 0), Ness::Pointi(3, 4), true);
	m_sprite->register_animator(ness_make_ptr<Ness::Animators::AnimatorSprite>(m_sprite, Ness::Sizei(FIREBALL_ANIM_STEPS, FIREBALL_DIRECTIONS_COUNT), 
		Ness::Pointi(0, direction), FIREBALL_ANIM_STEPS, 10.0f, Ness::Animators::SPRITE_ANIM_END_REPEAT));


	// distance left before self-terminate
	m_distance_left = 1000.0f;

	// create light
	m_light_node = lightNode;
	m_light = lightNode->create_light("../ness-engine/resources/gfx/light_round.jpg", Ness::Color::RED);
}

Fireball::~Fireball()
{
	m_sprite->parent()->remove(m_sprite);
	m_light_node->remove(m_light);
}

void Fireball::do_animation(Ness::Renderer* renderer)
{
	
	// move fireball
	float factor = 350.0f;
	switch (m_direction)
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

	// reduce distance left to go
	m_distance_left -= renderer->time_factor() * factor;
	if (m_distance_left < 0.0f)
		remove_from_animation_queue();

	// fix zindex
	m_sprite->set_zindex(m_sprite->get_position().y + 50);

	// fix light position
	m_light->set_position(m_sprite->get_position());
	m_light->set_zindex(m_sprite->get_zindex());
}