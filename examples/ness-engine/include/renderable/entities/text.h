/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

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
	// possible text alignment
	// basically affect anchor
	enum ETextAlignment
	{
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_RIGHT,
		TEXT_ALIGN_CENTER,
	};

	// the renderable text object
	// NOTE: does not support multiline. for multiline text used MultiText object.
	class Text : public Entity
	{
	protected:
		ManagedResources::ManagedFontPtr		m_font;					// the font used for this text
		String									m_text;					// the text itself
		bool									m_need_text_update;		// does this text need an update (after it was changed)
		SDL_Texture*							m_texture;				// the texture that renders this text
		unsigned int							m_line_width;			// max line width for this text
		Color									m_shadow;				// text shadow (set opacity 0.0f to disable)
		Pointi									m_shadow_offset;		// shadow offset


	public:

		// create the text object
		NESSENGINE_API Text(Renderer* renderer, ManagedResources::ManagedFontPtr font, const String& text);
		NESSENGINE_API Text(Renderer* renderer, const String& FontFile, const String& text, unsigned int font_size = 12);

		// clear the text
		NESSENGINE_API ~Text();

		// change font
		NESSENGINE_API inline void change_font(ManagedResources::ManagedFontPtr NewFont) {m_font = NewFont; m_need_text_update = true;}

		// set line width (in pixel). if text pass this limit, it will break line. give 0 to cancel line width limit
		NESSENGINE_API inline void set_line_width(unsigned int width) {m_line_width = width; m_need_text_update = true;}
		NESSENGINE_API inline unsigned int get_line_width() const {return m_line_width;}

		// change the text.
		NESSENGINE_API void change_text(const String& text) {m_text = text; m_need_text_update = true;}
		NESSENGINE_API const String& get_text() {return m_text;}

		// set text alignment
		NESSENGINE_API void set_alignment(ETextAlignment align);

		// return the texture this sprite uses
		NESSENGINE_API inline const ManagedResources::ManagedFontPtr& get_font() const {return m_font;}
		NESSENGINE_API inline ManagedResources::ManagedFontPtr get_font() {return m_font;}

		// add shadow effect to this text
		NESSENGINE_API void set_shadow(const Color& color, const Pointi& offset);
		NESSENGINE_API void disable_shadow() {m_shadow.a = 0.0f;}

		// override render so we update text first if we need to
		NESSENGINE_API void render(const CameraPtr& camera = NullCamera);

	protected:

		// update after font/text change
		NESSENGINE_API void update();

		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// sprite pointer type
	NESSENGINE_API typedef SharedPtr<Text> TextPtr;
};