/**
* A renderable text with multilines support
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "text.h"
#include "entity.h"
#include "../node_api.h"

namespace Ness
{

	// the renderable text object that support multilines (seperate lines with \n)
	class MultiText : public NodeAPI, public Entity
	{
	private:
		Ness::Vector<TextPtr>					m_lines;
		String									m_text;
		unsigned int							m_line_width;
		unsigned int							m_font_size;
		String									m_font;

	public:
		// create the multiline text
		NESSENGINE_API MultiText(Renderer* renderer, const String& FontFile, const String& text, int fontSize = 12);

		// set the text
		NESSENGINE_API void set_text(const String& text);
	};
};