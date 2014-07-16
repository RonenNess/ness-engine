/**
* A renderable text
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "entity.h"
#include "../../managed_resources/managed_font.h"
#include "../../basic_types/rectangle.h"

namespace Ness
{

	// the renderable text class
	class Text : public Entity
	{
	protected:
		ManagedResources::ManagedFontPtr		m_font;
		std::string								m_text;
		bool									m_need_text_update;
		SDL_Texture*							m_texture;
		unsigned int							m_line_width;

	public:

		// create the sprite with texture
		NESSENGINE_API Text(Renderer* renderer, ManagedResources::ManagedFontPtr font, const std::string& text);
		NESSENGINE_API Text(Renderer* renderer, const std::string& FontFile, const std::string& text, int fontSize = 12);

		// change font
		NESSENGINE_API inline void change_font(ManagedResources::ManagedFontPtr NewFont) {m_font = NewFont; m_need_text_update = true;}

		// set line width (in pixel). if text pass this limit, it will break line. give 0 to cancel line width limit
		NESSENGINE_API inline void set_line_width(unsigned int width) {m_line_width = width;}
		NESSENGINE_API inline unsigned int get_line_width() const {return m_line_width;}

		// change text
		NESSENGINE_API void change_text(const std::string& text) {m_text = text; m_need_text_update = true;}
		NESSENGINE_API const std::string& get_text() {return m_text;}

		// return the texture this sprite uses
		NESSENGINE_API inline const ManagedResources::ManagedFontPtr& get_font() const {return m_font;}
		NESSENGINE_API inline ManagedResources::ManagedFontPtr get_font() {return m_font;}

	protected:

		// update after font/text change
		void update();

		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// sprite pointer type
	NESSENGINE_API typedef NessSharedPtr<Text> TextPtr;
};