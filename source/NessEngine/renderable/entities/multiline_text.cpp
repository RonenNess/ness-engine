#include "multiline_text.h"

namespace Ness
{
	MultiText::MultiText(Renderer* renderer, const String& FontFile, const String& text, int fontSize) : NodeAPI(renderer), Entity(renderer)
	{
		m_line_width = 0;
		m_font = FontFile;
		m_font_size = fontSize;
		set_text(text);
	}

	void MultiText::set_text(const String& text)
	{

	}
};