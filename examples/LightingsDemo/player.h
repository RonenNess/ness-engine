#include <NessEngine.h>

enum EPlayerAction
{
	PLAYER_STANDING,
	PLAYER_WALKING,
	PLAYER_JUMPING,
};

enum EDirection
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
};

class Player
{
private:
	Ness::SpritePtr m_sprite;
	Ness::LightPtr m_light;
	Ness::LightPtr m_round_light;
	EPlayerAction m_action;
	Ness::Animators::AnimatorPtr m_animation;
	EDirection m_direction;
	float m_light_direction;

public:
	// create the player
	Player(Ness::NodePtr parent, Ness::LightNodePtr lightNode);

	// do every-frame events (handle actions and gravity)
	void do_events();

	// walk player left
	void walk_left();
	void walk_right();

	// stop walking (stand)
	void stand();

	// aim light up/down
	void aim_light_up();
	void aim_light_down();

private:

	// put light direction in legal range
	void put_light_in_range();

	// called whenever player changes action
	void change_action(EPlayerAction newAction);
};