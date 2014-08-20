#pragma once

#include <NessEngine.h>
#include <Box2D/Box2D.h>

// ratio between meter and pixels. 1 meter == 100 pixels.
#define METERS_TO_PIXELS 100.0f

// convert from box2d stuff to ness-engine stuff
class Box2dToNess
{
public:
	// convert box2d to ness point
	static Ness::Point point(const b2Vec2& point) {return Ness::Point(point.x * METERS_TO_PIXELS, point.y * METERS_TO_PIXELS); }

	// convert box2d to ness float
	static float scalar(const float32 scalar) {return scalar * METERS_TO_PIXELS; }

	// convert box2d to ness angle (radian to degree)
	static int angle(const float32 radians) {return (int)RADIAN_TO_DEGREE(radians);}
};

// convert from ness-engine stuff to box2d stuff
class NessToBox2d
{
public:
	// convert ness to box2d point
	static b2Vec2 point(const Ness::Point& point) {return b2Vec2(point.x / METERS_TO_PIXELS, point.y / METERS_TO_PIXELS); }

	// convert ness to box2d float
	static float32 scalar(const float scalar) {return scalar / METERS_TO_PIXELS; }

	// convert ness to box2d angle (degree to radian)
	static float32 angle(const int angle) {return (float)DEGREE_TO_RADIAN(angle);}
};

// connect Box2D body to a ness-engine entity
class NessBoxConnector
{
private:
	Ness::RenderablePtr m_ness_entity;
	b2Body*				m_box_body;

public:
	// create the connector
	NessBoxConnector(const Ness::RenderablePtr& nessEntity, b2Body* box2Dbody) : m_ness_entity(nessEntity), m_box_body(box2Dbody)
	{ }

	// update position and rotation (must be called every frame)
	inline void update()
	{
		m_ness_entity->set_position(Box2dToNess::point(m_box_body->GetPosition()));
		m_ness_entity->set_rotation((float)RADIAN_TO_DEGREE(m_box_body->GetAngle()));
	}

	// set position
	inline void set_position(const Ness::Point& NewPos)
	{
		m_box_body->SetTransform(NessToBox2d::point(NewPos), m_box_body->GetAngle());
	}

	// set rotation
	inline void set_rotation(int NewAngle)
	{
		m_box_body->SetTransform(m_box_body->GetPosition(), NessToBox2d::angle(NewAngle));
	}
};