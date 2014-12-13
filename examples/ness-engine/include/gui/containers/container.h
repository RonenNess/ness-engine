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
* A gui frame is like a windows form - its a framed region you can
* add widgets to. for most gui cases you would want to create a gui frame and 
* add gui elements to it, rather then having gui elements "floating" around 
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

		// a gui frame that contain other widgets
		class Container : public GuiContainerAPI
		{
		private:
			GuiElementPtr		m_focused_widget;		// the son widget currently under focus (or null if have no widget focused)
			Pointi				m_size;					// frame size in gui grid units
			TileMapPtr			m_graphics;				// the graphical part of the frame
			BoundingBox			m_bounding_box;			// the bounding box of this container
			bool				m_mouse_inside;			// is mouse currently inside this container
			bool				m_is_focused;			// does this container currently have focus?

		public:

			// create the frame widget
			// size_in_units:	is the container size in gui grid units (defined in the gui manager)
			// visual:			if true, will make this container visual with graphics. else, this container will not
			//					have graphical representation.
			NESSENGINE_API Container(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units, bool visual=true);

			// handle basic events
			NESSENGINE_API virtual void invoke_event_get_focus();
			NESSENGINE_API virtual void invoke_event_lose_focus();
			NESSENGINE_API virtual bool is_point_on(const Ness::Pointi& pos);
			NESSENGINE_API virtual void invoke_event_click(EMouseButtons mouse_button, const Ness::Pointi& mouse_pos);
			NESSENGINE_API virtual void invoke_event_key_down(EMouseButtons key);
			NESSENGINE_API virtual void invoke_event_key_up(EMouseButtons key);
			NESSENGINE_API virtual void invoke_event_mouse_enter(const Pointi& mouse_pos);
			NESSENGINE_API virtual void invoke_event_mouse_leave(const Pointi& mouse_pos);
			NESSENGINE_API virtual void invoke_event_mouse_hover(const Pointi& mouse_pos);
			NESSENGINE_API virtual void invoke_event_visibility_changed(bool newState);
			NESSENGINE_API virtual void invoke_event_enabled_changed(bool newState);

			// should return true if mouse is on this gui element
			NESSENGINE_API virtual bool is_mouse_on() {return m_mouse_inside;}

			// create a son container inside this container
			SharedPtr<Container> create_container(const Pointi& size_in_units);

			// render the container
			NESSENGINE_API virtual void render();

			// return the element's bounding box
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() {return m_bounding_box;}
			
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<Container> ContainerPtr;
	}
}