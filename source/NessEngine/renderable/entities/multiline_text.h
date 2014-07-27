/**
* A renderable text with multilines support
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "text.h"
#include "entity.h"
#include "../nodes/basic_node.h"

namespace Ness
{

	// the renderable text object that support multilines (seperate lines with \n)
	class MultiText : public Entity
	{
	private:
		Vector<TextPtr>							m_lines;					// string lines and part of lines (if break sign)
		unsigned int							m_line_width;				// max line width
		unsigned int							m_font_size;				// font size
		String									m_font;						// font to use by default
		ETextAlignment							m_alignement;				// alignment to all the string parts
		bool									m_need_text_positioning;	// if true, means we need to rearrange the text part positions
		BaseNodePtr								m_node;						// the node used to store all the string parts


	public:
		// create the multiline text
		NESSENGINE_API MultiText(Renderer* renderer, const String& FontFile, const String& text, int fontSize = 12);

		// set the text
		NESSENGINE_API void set_text(const String& text);

		// return the text lines
		NESSENGINE_API inline const Vector<TextPtr>& get_lines() const {return m_lines;}

		// set alignment
		NESSENGINE_API void set_alignment(ETextAlignment align) {m_alignement = align; m_need_text_positioning = true;}

		// when transformations update
		NESSENGINE_API virtual void transformations_update();

		// check if at least one of the lines are really visible
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// check if at least one of the lines are really visible, without effecting their internal states
		NESSENGINE_API virtual bool is_really_visible_const(const CameraPtr& camera) const;

		// render this text
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

		// change the parent of this object
		NESSENGINE_API virtual void __change_parent(NodeAPI* parent)
		{
			Entity::__change_parent(parent);
			m_node->__change_parent(parent);
		}

	protected:

		// update all the text lines position based on scale etc..
		NESSENGINE_API void update_lines_positions();

		// the actual rendering function - irrelevant for multitext object
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations) {}

		// irrelevant for multitext object
		NESSENGINE_API virtual void calc_target_rect() {}
	};

	NESSENGINE_API typedef SharedPtr<MultiText> MultiTextPtr;
};