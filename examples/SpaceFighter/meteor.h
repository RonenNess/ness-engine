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

	// set meteor position
	inline void set_position(const Ness::Point& pos) {m_meteor->set_position(pos);}
	inline const Ness::Point& get_position() const {return m_meteor->get_position();}

	// animate the meteor and move it.
	void do_events();

};