/*
* Adapter between ness-engine and box2d, to create objects with physics.
* basically the adapter is a connector that gets both a ness-engine entity and a box2d body and update them accordingly.
* Athor: Ronen Ness
* Since: 10/14
*/

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
	static float angle(const float32 radians) {return (float)RADIAN_TO_DEGREE(radians);}

	// convert box2d to ness color
	static Ness::Color color(const b2Color& color) {return Ness::Color(color.r, color.g, color.b, 1.0f);}
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
	static float32 angle(const float angle) {return (float)DEGREE_TO_RADIAN(angle);}

	// convert box2d to ness color
	static b2Color color(const Ness::Color& color) {return b2Color(color.r, color.g, color.b);}
};

// connect Box2D body to a ness-engine entity
class NessBoxConnector : public Ness::Animators::AnimatorAPI
{
private:
	Ness::RenderablePtr m_ness_entity;
	b2Body*				m_box_body;

public:
	// create the connector
	NessBoxConnector(const Ness::RenderablePtr& nessEntity, b2Body* box2Dbody) : m_ness_entity(nessEntity), m_box_body(box2Dbody)
	{ }

	// get entity/body
	inline Ness::RenderablePtr& get_entity() {return m_ness_entity;}
	inline const Ness::RenderablePtr& get_entity() const {return m_ness_entity;}
	inline b2Body* get_body() {return m_box_body;}
	inline const b2Body* get_body() const {return m_box_body;}

	// update position and rotation (must be called every frame)
	inline void do_animation(Ness::Renderer* renderer)
	{
		// set position
		m_ness_entity->set_position(Box2dToNess::point(m_box_body->GetPosition()));

		// set rotation
		m_ness_entity->set_rotation(Box2dToNess::angle(m_box_body->GetAngle()));
	}

	// set position
	inline void set_position(const Ness::Point& NewPos)
	{
		m_box_body->SetTransform(NessToBox2d::point(NewPos), m_box_body->GetAngle());
	}

	// set rotation
	inline void set_rotation(float NewAngle)
	{
		m_box_body->SetTransform(m_box_body->GetPosition(), NessToBox2d::angle(NewAngle));
	}
};

