#include "player.h"

// function to remove all shots that ran out of time to live
bool remove_dead_shots(const std::auto_ptr<LaserShot>& shot)
{
	return shot->get_time_to_live() <= 0.0f;
}

Player::Player(Ness::NodePtr& parentNode, const Ness::String& spacehipTexture, const Ness::Color& fireColor) : 
m_acceleration(1.25f), m_turning_speed(250.0f), m_max_speed(2.0f), m_parent(parentNode), m_speed(0.0f, 0.0f), 
	m_fire_color(fireColor), m_fire_cooldown(0.0f), m_shooting_rate(2.0f)
{
	// create the node that will contain all player spaceship parts
	m_node = parentNode->create_node();
	m_node->set_scale(0.75f);

	m_spaceship = m_node->create_sprite(spacehipTexture);
	m_spaceship->set_anchor(Ness::Point(0.5f, 0.5f));
	m_spaceship->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// create the player spaceship backfire effect when flying forward
	m_backFire = m_node->create_sprite("player_backfire.png");
	m_backFire->set_anchor(Ness::Point(0.5f, 0.5f));
	m_backFire->set_blend_mode(Ness::BLEND_MODE_BLEND);
	m_backFire->set_color(m_fire_color);

	// create the player spaceship frontfire effect when flying backwards
	m_frontFire = m_node->create_sprite("player_frontfire.png");
	m_frontFire->set_anchor(Ness::Point(0.5f, 0.5f));
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

	// move shots
	for (auto shot = m_shots.begin(); shot != m_shots.end(); ++shot)
	{
		(*shot)->do_events();
	}
	m_shots.remove_if(remove_dead_shots);

}

void Player::do_collisions(const std::list<std::auto_ptr<Meteor> >& meteors, Player& other)
{
	// check collision for our shots
	for (auto shot = m_shots.begin(); shot != m_shots.end(); ++shot)
	{
		// shot with other player
		if ((*shot)->get_position().distance(other.get_position()) < other.get_radius())
		{
			other.apply_force((*shot)->get_direction_vector().get_normalized() * 0.1f);
			(*shot)->destroy(true);
			continue;
		}

		// shot with meteors
		for (auto meteor = meteors.begin(); meteor != meteors.end(); ++meteor)
		{
			if ((*shot)->get_position().distance((*meteor)->get_position()) < (*meteor)->get_radius())
			{
				(*meteor)->apply_force((*shot)->get_direction_vector().get_normalized() * 10.0f);
				(*shot)->destroy(true);
			}
		}
	}

	// check collision between our spaceship and meteors
	for (auto meteor = meteors.begin(); meteor != meteors.end(); ++meteor)
	{
		float distance_from_meteor = get_position().distance((*meteor)->get_position());
		if (distance_from_meteor < (*meteor)->get_radius() + get_radius() * 0.65f)
		{
			apply_force((get_position() - (*meteor)->get_position()) * m_node->renderer()->time_factor() * 0.05f);
		}
	}
}

void Player::turn_left()
{
	m_node->set_rotation(m_node->get_rotation() - m_node->renderer()->time_factor() * m_turning_speed);
}

void Player::turn_right()
{
	m_node->set_rotation(m_node->get_rotation() + m_node->renderer()->time_factor() * m_turning_speed);
}

void Player::apply_force(const Ness::Point& force)
{
	// apply the force and make sure speed is within the limit
	m_speed += force;
	m_speed.limit(-m_max_speed, m_max_speed);
}

void Player::fly_forward()
{
	// get movement vector and add to speed
	Ness::Point moveVector = Ness::Point::from_angle((int)m_node->get_rotation(), (m_node->renderer()->time_factor() * m_acceleration));
	apply_force(moveVector);

	// do backfire effect
	m_backFire->set_visible((rand() % 5) > 1);
}

void Player::fly_backwards()
{
	// get movement vector and add to speed
	Ness::Point moveVector = Ness::Point::from_angle((int)m_node->get_rotation(), (m_node->renderer()->time_factor() * m_acceleration));
	apply_force(-moveVector);

	// do frontfire effect
	m_frontFire->set_visible((rand() % 5) > 1);
}