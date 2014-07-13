#include "shot.h"

LaserShot::LaserShot(Ness::NodePtr& parentNode, const Ness::Point& position, float Direction, const Ness::Color& fireColor)
{
	m_shot = parentNode->create_sprite("shot.png");
	m_shot->set_anchor(Ness::Point::HALF);
	m_shot->set_color(fireColor);
	m_shot->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_shot->set_position(position);
	m_shot->set_rotation((float)Direction);
}