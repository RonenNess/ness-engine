#include <NessEngine.h>

enum EDirection
{
	DIRECTION_DOWN = 0,
	DIRECTION_LEFT = 1,
	DIRECTION_RIGHT = 2,
	DIRECTION_UP = 3,
};

// represent a playable character
class Character : public Ness::Animators::AnimatorAPI
{
private:
	Ness::NodePtr						m_parent;
	Ness::LightNodePtr					m_light_node;
	Ness::LightPtr						m_light;
	Ness::SpritePtr						m_sprite;
	Ness::Animators::AnimatorSpritePtr	m_animator;
	EDirection							m_last_direction;
	bool								m_is_walking;
	float								m_fire_cooldown;

public:
	// create the character
	Character(Ness::LightNodePtr& lightNode, Ness::NodePtr& parent, const Ness::String& sprite);

	// stop walking
	void stop();

	// walk to direction
	void walk(EDirection direction);

	// shoot fireball
	void shoot();

	// set/get position
	inline void set_position(const Ness::Point& pos) {m_sprite->set_position(pos);}
	inline const Ness::Point& get_position() const {return m_sprite->get_position();}
	inline Ness::Size get_absolute_size() const {return m_sprite->get_absolute_size();}
	inline void set_zindex(float z) {m_sprite->set_zindex(z);}

	// get the character light
	inline Ness::LightPtr& get_light() {return m_light;}

	// do animations every frame
	virtual void do_animation(Ness::Renderer* renderer);
};