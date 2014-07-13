#include "meteor.h"

Meteor::Meteor(Ness::NodePtr& parentNode, const Ness::Point& speed, float size) : m_speed(speed)
{
	m_meteor = parentNode->create_sprite("meteor.png");
	m_meteor->set_size(Ness::Size(size, size));
	m_meteor->set_anchor(Ness::Point::HALF);
	m_meteor->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_rotation_speed = 100.0f - (rand() % 200);
}

Meteor::~Meteor()
{
	m_meteor->parent()->remove(m_meteor);
}

void Meteor::do_events()
{
	Ness::Point MeteorPos = m_meteor->get_position();
	MeteorPos += m_speed * m_meteor->renderer()->time_factor() * 100.0f;;
	m_meteor->set_position(MeteorPos);
	float NewAngle = m_meteor->get_rotation() + m_rotation_speed * m_meteor->renderer()->time_factor();
	m_meteor->set_rotation(NewAngle);
}

void Meteor::apply_force(const Ness::Point& force)
{
	m_speed += force;
}