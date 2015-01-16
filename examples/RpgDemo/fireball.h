#include <NessEngine.h>

// represent a flying fireball
class Fireball : public Ness::Animators::AnimatorAPI
{
private:
	Ness::LightNodePtr				m_light_node;
	Ness::LightPtr					m_light;
	Ness::NodePtr					m_parent;
	Ness::AnimatedSpritePtr			m_sprite;
	bool							m_is_walking;
	int								m_direction;
	float							m_distance_left;

public:
	Fireball(Ness::LightNodePtr& lightNode, Ness::NodePtr& parent, const Ness::Point& position, const int direction);
	~Fireball();

	virtual void do_animation(Ness::Renderer* renderer);
};