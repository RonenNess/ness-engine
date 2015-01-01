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
* A gui frame is the graphical component of a widget container.
* this is mostly used internally, to create the background of a button, a container, and other widgets.
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once


#include "gui_widget_api.h"
#include "../../renderable/entities/text.h"
#include "../../renderable/nodes/tile_map.h"

namespace Ness
{
	namespace Gui
	{

		// a graphical frame widget. this object is used mostly internally, as the
		// background/frame of buttons, containers, etc..
		class Frame : public WidgetAPI
		{
		private:
			String				m_textures_prefix;		// the textures prefix for this frame: "frame", "button", etc..
			Point				m_position;				// the frame position relative to its parent, in pixels
			Pointi				m_size;					// frame size in gui grid units
			TileMapPtr			m_graphics;				// the graphical part of the frame
			BoundingBox			m_bounding_box;			// the bounding box of this frame
			bool				m_mouse_inside;			// is mouse currently inside the frame?
			bool				m_is_focused;			// does this frame currently have focus?
			
		public:

			// create the frame widget
			// size_in_units:	is the container size in gui grid units (defined in the gui manager)
			NESSENGINE_API Frame(GuiManager* manager, GuiContainerAPI* parent, 
				const Pointi& size_in_units, const String& texture_prefix = "frame");
			NESSENGINE_API ~Frame();

			// handle basic events
			NESSENGINE_API virtual void __invoke_event_get_focus();
			NESSENGINE_API virtual void __invoke_event_lose_focus();
			NESSENGINE_API virtual void __invoke_event_mouse_down(EMouseButtons button, const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_up(EMouseButtons button, const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_enter(const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_leave(const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_hover(const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_visibility_changed(bool new_state, bool by_parent);
			NESSENGINE_API virtual void __invoke_event_enabled_changed(bool new_state, bool by_parent);
			NESSENGINE_API virtual void __invoke_event_update_position();

			// set container position, relative to parent, in pixels
			NESSENGINE_API virtual void set_position(const Point& new_pos, const Point& anchor = Point::ZERO);

			// should return true if mouse is on this gui element
			NESSENGINE_API virtual bool is_mouse_on() {return m_mouse_inside;}

			// recalculate container bounding box (you should not call it on your own it happens automatically)
			NESSENGINE_API virtual void calculate_bounding_box();

			// return the element's bounding box
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() const {return m_bounding_box;}			

			// set the color of this frame
			NESSENGINE_API void set_color(const Color& color) {m_graphics->set_color(color);}
			NESSENGINE_API const Color& get_color() {return m_graphics->get_color();}

		private:
			// update frame texture based on state
			// state should be like "focused", "unfocused", "mouse_down", etc..
			// check out the gui graphics dir that comes with ness-engine for more info
			NESSENGINE_API void update_texture(const String& state);
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<Frame> FramePtr;
	}
}