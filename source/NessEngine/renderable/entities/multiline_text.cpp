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
		set_text(text);
	}

	void MultiText::set_text(const String& text)
	{
		// loop over the string split by \n character
		std::istringstream f(text);
		String current;    
		while (getline(f, current, '\n')) {
			
			// create the text line
			TextPtr text = TextPtr(new Text(Entity::m_renderer, m_font, current, m_font_size));
			m_lines.push_back(text);
			text->__change_parent(m_node.get());
		}

		// required text positioning
		m_need_text_positioning = true;
	}

	// update all the text lines position based on scale etc..
	void MultiText::update_lines_positions()
	{
		float LineHeight = m_font_size * get_absolute_size().y;
		for (unsigned int i = 0; i < m_lines.size(); ++i)
		{
			m_lines[i]->set_position(Point(0.0f, LineHeight));
			m_lines[i]->set_alignment(m_alignement);
		}
	}

	void MultiText::transformations_update()
	{
		Entity::transformations_update();
		m_need_text_positioning = true;
		m_node->set_transformations(m_transformations);
	}

	void MultiText::render(const CameraPtr& camera)
	{
		if (m_need_text_positioning)
			update_lines_positions();
		
		for (unsigned int i = 0; i < m_lines.size(); ++i)
			m_lines[i]->render(camera);
	}

	// this rendering function is blank for multitask entity, since everything is done in render()
	void MultiText::do_render(const Rectangle& target, const SRenderTransformations& transformations)
	{
	}
};