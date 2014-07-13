#pragma once
#include <NessEngine.h>

// define the laser shot object that players can shoot
class LaserShot
{
private:
	Ness::SpritePtr m_shot;		// the shot itself

public:
	LaserShot(Ness::NodePtr& parentNode, const Ness::Point& position, float Direction, const Ness::Color& fireColor);
};