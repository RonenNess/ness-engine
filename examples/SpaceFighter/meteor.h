#pragma once
#include <NessEngine.h>

// define the meteor object
class Meteor
{
private:
	Ness::Point m_speed;			// meteor speed
	Ness::SpritePtr m_meteor;		// meteor sprite

public:
	// create the new meteor
	Meteor(Ness::NodePtr parentNode, const Ness::Point& speed, float size);
};