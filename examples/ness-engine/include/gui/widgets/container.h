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
#include "gui_element_api.h"
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

		public:

			// create the frame widget
			// size_in_units is the container size in gui grid units (defined in the gui manager)
			NESSENGINE_API Container(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units);

			// handle basic events
			NESSENGINE_API virtual void invoke_event_get_focus();
			NESSENGINE_API virtual void invoke_event_lose_focus();
			NESSENGINE_API virtual bool is_point_on(const Ness::Pointi& pos);
			NESSENGINE_API virtual void invoke_event_click(EMouseButtons mouse_button, const Ness::Pointi& mouse_pos);
			NESSENGINE_API virtual void invoke_event_key_down(EMouseButtons key);
			NESSENGINE_API virtual void invoke_event_key_up(EMouseButtons key);
			NESSENGINE_API virtual void invoke_event_mouse_enter();
			NESSENGINE_API virtual void invoke_event_mouse_leave();

			// return the element's bounding box
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() {return m_bounding_box;}
			
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<Container> ContainerPtr;
	}
}