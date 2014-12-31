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
* A gui container is like a windows form - its a framed region you can
* add widgets to. for most gui cases you would want to create a container and 
* add widgets to it, rather then having gui elements "floating" around 
* the screen.
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "gui_container_api.h"
#include "../../renderable/nodes/tile_map.h"

namespace Ness
{
	namespace Gui
	{
		// docking options for a container
		enum EDockingOptions
		{
			DOCK_DISABLED,
			DOCK_TOP_CENTER,
			DOCK_TOP_LEFT,
			DOCK_TOP_RIGHT,
			DOCK_BOTTOM_CENTER,
			DOCK_BOTTOM_LEFT,
			DOCK_BOTTOM_RIGHT,
			DOCK_LEFT_CENTER,
			DOCK_RIGHT_CENTER,
			DOCK_CENTER,
		};

		
		// predeclare all the entities this container can create
		class Label;
		NESSENGINE_API typedef SharedPtr<Label> LabelPtr;
		class Frame;
		NESSENGINE_API typedef SharedPtr<Frame> FramePtr;
		
		// a gui container that wraps and contain other widgets
		class Container : public GuiContainerAPI
		{
		private:
			NodePtr				m_node;					// the node used by this container
			Point				m_position;				// the container position relative to its parent, in pixels
			GuiElementPtr		m_focused_widget;		// the son widget currently under focus (or null if have no widget focused)
			Pointi				m_size;					// frame size in gui grid units
			FramePtr			m_graphics;				// the graphical part of the frame
			BoundingBox			m_bounding_box;			// the bounding box of this container
			bool				m_mouse_inside;			// is mouse currently inside this container
			bool				m_is_focused;			// does this container currently have focus?
			EDockingOptions		m_docking;				// if not DOCK_DISABLED will dock this container to a given anchor

		public:

			// create the frame widget
			// size_in_units:	is the container size in gui grid units (defined in the gui manager)
			// visual:			if true, will make this container visual with graphics. else, this container will not
			//					have graphical representation.
			NESSENGINE_API Container(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units, bool visual=true);

			// handle basic events
			NESSENGINE_API virtual void __invoke_event_get_focus();
			NESSENGINE_API virtual void __invoke_event_lose_focus();
			NESSENGINE_API virtual void __invoke_event_click(EMouseButtons mouse_button, const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_key_down(EMouseButtons key);
			NESSENGINE_API virtual void __invoke_event_key_up(EMouseButtons key);
			NESSENGINE_API virtual void __invoke_event_mouse_enter(const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_leave(const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_hover(const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_visibility_changed(bool new_state, bool by_parent);
			NESSENGINE_API virtual void __invoke_event_enabled_changed(bool new_state, bool by_parent);
			NESSENGINE_API virtual void __invoke_event_update_position();
			NESSENGINE_API virtual void __invoke_event_mouse_pressed(EMouseButtons button, const Pointi& mouse_pos);
			NESSENGINE_API virtual void __invoke_event_mouse_released(EMouseButtons button, const Pointi& mouse_pos);

			// return the node
			NESSENGINE_API virtual NodePtr get_node() {return m_node;}

			// set container position, relative to parent, in pixels
			NESSENGINE_API virtual void set_position(const Point& new_pos, const Point& anchor = Point::ZERO);

			// set position of this widget to dock given position (relative to parent)
			NESSENGINE_API inline void dock_to(EDockingOptions dock_to) {m_docking = dock_to; __invoke_event_update_position();}

			// should return true if mouse is on this gui element
			NESSENGINE_API virtual bool is_mouse_on() {return m_mouse_inside;}

			// create a son container inside this container
			NESSENGINE_API SharedPtr<Container> create_container(const Pointi& size_in_units);
			// create and return a label
			NESSENGINE_API LabelPtr create_label(const String& text);

			// recalculate container bounding box (you should not call it on your own it happens automatically)
			NESSENGINE_API virtual void calculate_bounding_box();

			// return the element's bounding box
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() const {return m_bounding_box;}

		private:
			// fix container docking (use dock_to() to set docking mode)
			NESSENGINE_API void fix_docking();

			// update the widget under focus based on mouse position
			NESSENGINE_API void set_focus_on(const Point& mouse_pos);
			
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<Container> ContainerPtr;
	}
}