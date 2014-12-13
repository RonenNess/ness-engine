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
* The root object that manage a gui system.
* define the basic flags and definitions of the gui and you create gui widgets and nodes throught it.
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once
#include "widgets\all_widgets.h"
#include "containers\all_containers.h"

namespace Ness
{
	namespace Gui
	{
		// a special scene node to hold and manage gui elements
		class GuiManager
		{
		protected:
			Containers::Vector<ContainerPtr>	m_containers;			// the gui containers in this manager
			Renderer*							m_renderer;				// pointer to the renderer object
			Point								m_unit_size;			// all widgets size and position are based on unit size (like a grid)
			String								m_resources_path;		// the path of the folder in which we have the gui textures

		public:
			// create the gui manager
			// renderer: pointer to the owner renderer
			// resources_path: the path to the folder that will contain all the gui textures. 
			//					there is default gui skin provided within ness-engine resources.
			// grid_unit_size: all sizes and units in gui elements are based on the grid. this 
			//					param determine the size of a single grid unit.
			NESSENGINE_API GuiManager(Renderer* renderer, 
				const String& resources_path = "ness-engine/resources/gui/", 
				const Point& grid_unit_size = Point(32, 32)) 
				: m_renderer(renderer), m_unit_size(grid_unit_size), m_resources_path(resources_path + "/") {}

			// create a gui container
			// size_in_units is the size of the container in grid_unit_size (as defined when you created the gui manager)
			NESSENGINE_API ContainerPtr create_container(const Pointi& size_in_units);

			// get the renderer
			NESSENGINE_API inline Renderer* renderer() const {return m_renderer;}

			// get the unit size
			NESSENGINE_API inline const Point& get_unit_size() const {return m_unit_size;}

			// get the resources path
			NESSENGINE_API inline const String& get_resources_path() const {return m_resources_path;}

			// render the gui!
			NESSENGINE_API void render();
		};

		// a pointer to a gui node
		NESSENGINE_API typedef SharedPtr<GuiManager> GuiManagerPtr;
	}
}