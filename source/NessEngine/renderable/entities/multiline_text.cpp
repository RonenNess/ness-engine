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

#include "../../renderer/renderer.h"
#include "multiline_text.h"
#include <sstream>
#include <iostream>

namespace Ness
{
	MultiText::MultiText(Renderer* renderer, const String& FontFile, const String& text, int fontSize) : Entity(renderer)
	{
		m_node = BaseNodePtr(new BaseNode(renderer));
		m_need_text_positioning = true;
		m_line_width = 0;
		m_font = FontFile;
		m_font_size = fontSize;
		m_alignement = TEXT_ALIGN_LEFT;
		set_text(text);
	}

	void MultiText::set_text(const String& text)
	{
		// hold temporary pointer to the used font
		// this is so we won't get unnecessary font reloads when changing text
		ManagedResources::ManagedFontPtr font;
		if (m_lines.size() > 0)
		{
			font = m_lines[0]->get_font();
		}

		// clear previous lines
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			m_lines[i]->__change_parent(nullptr);
		}
		m_lines.clear();
		m_node->clear();

		// loop over the string split by \n character
		std::istringstream f(text);
		String current;    
		while (getline(f, current, '\n')) {
			
			// create the text line
			TextPtr text = TextPtr(new Text(Entity::m_renderer, m_font, current, m_font_size));
			text->__change_parent(m_node.get());
			text->set_static(m_static);
			m_lines.push_back(text);
		}

		// required text positioning
		m_need_text_positioning = true;
		transformations_update();
	}

	// update all the text lines position based on scale etc..
	void MultiText::update_lines_positions()
	{
		float LineHeight = m_font_size * get_absolute_transformations().scale.y;
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			Ness::TextPtr& curr = m_lines[i];
			curr->set_position(Point(0.0f, LineHeight * i));
			curr->set_alignment(m_alignement);
			curr->set_static(m_static);
		}
	}

	void MultiText::transformations_update()
	{
		m_node->set_transformations(m_transformations);
		Entity::transformations_update();
		m_need_text_positioning = true;
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			m_lines[i]->transformations_update();
		}
	}

	bool MultiText::is_really_visible(const CameraPtr& camera)
	{
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			if(m_lines[i]->is_really_visible(camera))
				return true;
		}
		return false;
	}

	bool MultiText::is_really_visible_const(const CameraPtr& camera) const
	{
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			if(m_lines[i]->is_really_visible_const(camera))
				return true;
		}
		return false;
	}

	void MultiText::render(const CameraPtr& camera)
	{
		if (m_need_text_positioning)
			update_lines_positions();
		
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			m_lines[i]->render(camera);
		}
	}
};