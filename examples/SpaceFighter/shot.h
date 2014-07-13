#pragma once
#include <NessEngine.h>

// define the laser shot object that players can shoot
class LaserShot
{
private:
	Ness::SpritePtr m_shot;
	Ness::Point m_direction_vector;
	float m_time_to_live;

public:
	// create the new shot instance
	LaserShot(Ness::NodePtr& parentNode, const Ness::Point& position, float Direction, const Ness::Color& fireColor);
	~LaserShot();

	// return how much time this shot got left
	inline float get_time_to_live() {return m_time_to_live;}
	void destroy(bool withExplosion);

	// return the shot direction vector
	inline const Ness::Point& get_direction_vector() const {return m_direction_vector;}

	// return the shot position
	inline const Ness::Point& get_position() const {return m_shot->get_position();}

	// move the shot and do its events
	void do_events();
};