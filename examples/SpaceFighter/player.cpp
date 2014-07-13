#include "player.h"

Player::Player(Ness::NodePtr& parentNode, const std::string& spacehipTexture, const Ness::Color& fireColor) : 
m_acceleration(1.25f), m_turning_speed(250.0f), m_max_speed(2.0f), m_parent(parentNode), m_speed(0.0f, 0.0f), 
	m_fire_color(fireColor), m_fire_cooldown(0.0f), m_shooting_rate(2.0f)
{
	// create the node that will contain all player spaceship parts
	m_node = parentNode->create_node();

	m_spaceship = m_node->create_sprite(spacehipTexture);
	m_spaceship->set_anchor(Ness::Point(0.5f, 0.5f));
	m_spaceship->set_scale(0.75f);
	m_spaceship->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// create the player spaceship backfire effect when flying forward
	m_backFire = m_node->create_sprite("player_backfire.png");
	m_backFire->set_anchor(Ness::Point(0.5f, 0.5f));
	m_backFire->set_scale(0.75f);
	m_backFire->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_backFire->set_color(m_fire_color);

	// create the player spaceship frontfire effect when flying backwards
	m_frontFire = m_node->create_sprite("player_frontfire.png");
	m_frontFire->set_anchor(Ness::Point(0.5f, 0.5f));
	m_frontFire->set_scale(0.75f);
	m_frontFire->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_frontFire->set_color(m_fire_color);
}

Player::~Player()
{
	// remove the spaceship node from the parent node. note: don't need to remove all the sprites, they will be removed automatically
	m_node->parent()->remove(m_node);
}

void Player::fire()
{
	if (m_fire_cooldown > 0.0f)
		return;
	m_fire_cooldown = 1.0f / m_shooting_rate;
	LaserShot* NewShot = new LaserShot(m_parent, m_node->get_position(), m_node->get_rotation(), m_fire_color);
	m_shots.push_back(std::auto_ptr<LaserShot>(NewShot));
}

void Player::do_events()
{
	// remove front fire / back fire effects
	if (m_frontFire->is_visible())
		m_frontFire->set_visible(false);
	if (m_backFire->is_visible())
		m_backFire->set_visible(false);

	// reduce cooldown time
	m_fire_cooldown -= m_node->renderer()->time_factor();

	// move the spaceship based on speed
	Ness::Point playerPos = m_node->get_position();
	playerPos += m_speed * m_node->renderer()->time_factor() * 100.0f;
	if (playerPos.x < 0.0f) playerPos.x = (float)m_node->renderer()->get_screen_size().x;
	if (playerPos.y < 0.0f)	playerPos.y = (float)m_node->renderer()->get_screen_size().y;
	if (playerPos.x > m_node->renderer()->get_screen_size().x) playerPos.x = 0.0f;
	if (playerPos.y > m_node->renderer()->get_screen_size().y) playerPos.y = 0.0f;
	m_node->set_position(playerPos);

}

void Player::turn_left()
{
	m_node->set_rotation(m_node->get_rotation() - m_node->renderer()->time_factor() * m_turning_speed);
}

void Player::turn_right()
{
	m_node->set_rotation(m_node->get_rotation() + m_node->renderer()->time_factor() * m_turning_speed);
}

void Player::fly_forward()
{
	// get movement vector and add to speed
	Ness::Point moveVector = Ness::Point::from_angle((int)m_node->get_rotation(), (m_node->renderer()->time_factor() * m_acceleration));
	m_speed += moveVector;

	// make sure speed is within limit
	m_speed.limit(-m_max_speed, m_max_speed);

	// do backfire effect
	m_backFire->set_visible((rand() % 5) > 1);
}

void Player::fly_backwards()
{
	// get movement vector and add to speed
	Ness::Point moveVector = Ness::Point::from_angle((int)m_node->get_rotation(), (m_node->renderer()->time_factor() * m_acceleration));
	m_speed -= moveVector;

	// make sure speed is within limit
	m_speed.limit(-m_max_speed, m_max_speed);

	// do frontfire effect
	m_frontFire->set_visible((rand() % 5) > 1);
}