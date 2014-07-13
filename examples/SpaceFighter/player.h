#pragma once
#include <NessEngine.h>

// define the player object
class Player
{
private:
	float m_acceleration;			// player acceleration
	float m_turning_speed;			// turning speed
	float m_max_speed;				// max speed
	Ness::Point m_speed;			// current speed
	Ness::NodePtr m_node;			// the node that contains all the spaceship parts
	Ness::SpritePtr m_spaceship;	// the spaceship sprite
	Ness::SpritePtr m_frontFire;	// front-fire effect when flying backwards
	Ness::SpritePtr m_backFire;		// back-fire effect when flying forward

public:
	// create the player and put it under parentNode
	Player(Ness::NodePtr parentNode, const std::string& spacehipTexture);

	// remove the player
	~Player();

	// set rotation
	inline void set_rotation(int angle) {m_node->set_rotation((float)angle);}

	// set position
	inline void set_position(const Ness::Point& pos) {m_node->set_position(pos);}

	// move the player based on speed, handle effects, ect..
	// NOTE: be sure to call do_events BEFORE doing movement (fly_forward/backwards) and other stuff
	void do_events();

	// give acceleration forward
	void fly_forward();

	// turn left
	void turn_left();

	// turn right
	void turn_right();

	// give acceleration backwards
	void fly_backwards();
};