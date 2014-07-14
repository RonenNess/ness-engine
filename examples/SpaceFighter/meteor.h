#pragma once
#include <NessEngine.h>

// define the meteor object
class Meteor
{
private:
	Ness::Point m_speed;			// meteor speed
	Ness::SpritePtr m_meteor;		// meteor sprite
	float m_rotation_speed;			// meteor rotation speed (and direction). randomize on creation.

public:
	// create the new meteor
	Meteor(Ness::NodePtr& parentNode, const Ness::Point& speed, float size);

	// destroy the meteor
	~Meteor();

	// return meteor size
	inline int get_size() const {return (int)m_meteor->get_size().x;}

	// get estimated radius for collision detection
	inline float get_radius() const {return get_size() * 0.65f;}

	// apply force that move this meteor
	void apply_force(const Ness::Point& force);

	// get speed
	inline const Ness::Point& get_speed() const {return m_speed;}

	// set meteor position
	inline void set_position(const Ness::Point& pos) {m_meteor->set_position(pos);}
	inline const Ness::Point& get_position() const {return m_meteor->get_position();}

	// animate the meteor and move it.
	void do_events();

};