#include <NessEngine.h>

enum EDirection
{
	DIRECTION_DOWN = 0,
	DIRECTION_LEFT = 1,
	DIRECTION_RIGHT = 2,
	DIRECTION_UP = 3,
};

// represent a playable character
class Character
{
private:
	Ness::NodePtr					m_parent;
	Ness::SpritePtr					m_sprite;
	Ness::Animators::AnimatorPtr	m_animator;
	EDirection						m_last_direction;
	bool							m_is_walking;

public:
	// create the character
	Character(Ness::NodePtr& parent, const Ness::String& sprite);

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
};