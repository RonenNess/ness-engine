#include "logo_show.h"
#include "../../renderable/entities/all_entities.h"
#include "../../renderer/renderer.h"
#include "../../animators/animator_fader.h"

namespace Ness
{
	namespace Utils
	{
		void make_logo_screen(const NodePtr& parentNode, 
			const String& textureName, 
			const Color& backColor, 
			float timeUntilFadeout, 
			float fadeOutSpeed)
		{
			// create the node for the logo screen
			NodePtr node = parentNode->create_node();
			node->set_blend_mode(BLEND_MODE_BLEND);

			// create the background colored rectangle
			RectangleShapePtr rect = node->create_rectangle();
			rect->set_anchor(Point::ZERO);
			rect->set_size(parentNode->renderer()->get_screen_size());
			rect->set_color(backColor);
			rect->set_static(true);
			
			// create the logo
			if (textureName.length())
			{
				SpritePtr sprite = node->create_sprite(textureName);
				sprite->set_anchor(Point::HALF);
				sprite->set_position(parentNode->renderer()->get_screen_center());
				sprite->set_static(true);
			}

			// add the fade-out animator
			Animators::AnimatorFaderOutPtr fader = ness_make_ptr<Animators::AnimatorFaderOut>(node, true, fadeOutSpeed, timeUntilFadeout);
			node->renderer()->register_animator(fader);
		}
	}
}