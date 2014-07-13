#pragma once
#include <NessEngine.h>
#include "shot.h"
#include "meteor.h"
#include <memory>

// define the player object
class Player
{
private:
	float m_acceleration;							// player acceleration
	float m_turning_speed;							// turning speed
	float m_max_speed;								// max speed
	Ness::NodePtr& m_parent;						// parent node
	Ness::Point m_speed;							// current speed
	Ness::NodePtr m_node;							// the node that contains all the spaceship parts
	Ness::SpritePtr m_spaceship;					// the spaceship sprite
	Ness::SpritePtr m_frontFire;					// front-fire effect when flying backwards
	Ness::SpritePtr m_backFire;						// back-fire effect when flying forward
	Ness::Color	m_fire_color;						// backfire/shooting color
	std::list<std::auto_ptr<LaserShot> > m_shots;	// all the shots this player fired and currently in the air
	float m_fire_cooldown;							// time until can shoot again
	float m_shooting_rate;							// shooting rate - how many shots per second

public:
	// create the player and put it under parentNode
	Player(Ness::NodePtr& parentNode, const std::string& spacehipTexture, const Ness::Color& fireColor);

	// remove the player
	~Player();

	// fire weapon
	void fire();

	// set rotation
	inline void set_rotation(int angle) {m_node->set_rotation((float)angle);}

	// set position
	inline void set_position(const Ness::Point& pos) {m_node->set_position(pos);}
	inline const Ness::Point& get_position() const {return m_node->get_position();}

	// apply force that move this player spaceship
	void apply_force(const Ness::Point& force);

	// move the player based on speed, handle effects, ect..
	// NOTE: be sure to call do_events BEFORE doing movement (fly_forward/backwards) and other stuff
	void do_events();

	// get estimated radius for collision detection
	inline float get_radius() const {return m_spaceship->get_size().x * m_node->get_scale().x * 0.45f;}

	// give acceleration forward
	void fly_forward();

	// turn left
	void turn_left();

	// turn right
	void turn_right();

	// give acceleration backwards
	void fly_backwards();

	// check collision with meteors and other players.
	// check both the player itself and his shots
	void do_collisions(const std::list<std::auto_ptr<Meteor> >& meteors, Player& other);
};