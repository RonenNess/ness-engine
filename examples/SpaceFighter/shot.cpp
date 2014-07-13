#include "shot.h"

LaserShot::LaserShot(Ness::NodePtr& parentNode, const Ness::Point& position, float Direction, const Ness::Color& fireColor)
{
	m_shot = parentNode->create_sprite("shot.png");
	m_shot->set_anchor(Ness::Point::HALF);
	m_shot->set_color(fireColor);
	m_shot->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_shot->set_position(position);
	m_shot->set_rotation(Direction);
	m_direction_vector = Ness::Point::from_angle((int)Direction);
	m_time_to_live = 0.75f;
}

void LaserShot::destroy(bool withExplosion) 
{
	m_time_to_live = 0.0f;
}

LaserShot::~LaserShot()
{
	m_shot->parent()->remove(m_shot);
}

void LaserShot::do_events()
{
	// move the shot based on speed, and wrap if out of screen
	Ness::Point shotPos = m_shot->get_position();
	shotPos += m_direction_vector * m_shot->renderer()->time_factor() * 350.0f;
	if (shotPos.x < 0.0f) shotPos.x = (float)m_shot->renderer()->get_screen_size().x;
	if (shotPos.y < 0.0f)	shotPos.y = (float)m_shot->renderer()->get_screen_size().y;
	if (shotPos.x > m_shot->renderer()->get_screen_size().x) shotPos.x = 0.0f;
	if (shotPos.y > m_shot->renderer()->get_screen_size().y) shotPos.y = 0.0f;
	m_shot->set_position(shotPos);

	// decrease shot time to live
	m_time_to_live -= m_shot->renderer()->time_factor();
	if (m_time_to_live < 0.1f)
		m_shot->set_scale(m_time_to_live * 10.0f);
}