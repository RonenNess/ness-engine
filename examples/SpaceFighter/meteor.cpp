#include "meteor.h"

Meteor::Meteor(Ness::NodePtr parentNode, const Ness::Point& speed, float size) : m_speed(speed)
{
	m_meteor = parentNode->create_sprite("meteor.png");
	m_meteor->set_scale(size);
}