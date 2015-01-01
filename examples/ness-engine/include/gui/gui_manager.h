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
#include "widgets/all_widgets.h"
#include "containers/all_containers.h"
#include "../utils/events/event_handler.h"
#include "../utils/events/mouse.h"
#include "../utils/events/keyboard.h"
#include "../managed_resources/managed_font.h"
#include "../managed_resources/managed_texture.h"

namespace Ness
{
	namespace Gui
	{
		// manage a gui system
		// note: you must register it to an event poller to make it react with keyboard and mouse!
		class GuiManager : public Utils::EventsHandler
		{
		protected:
			RootContainerPtr										m_root_container;				// the gui root container that contains all sub containers and elements
			Renderer*												m_renderer;						// pointer to the renderer object
			Sizei													m_unit_size;					// all widgets size and position are based on unit size (like a grid)
			String													m_resources_path;				// the path of the folder in which we have the gui textures
			Utils::Mouse											m_mouse;						// mouse events handler
			Utils::Keyboard											m_keyboard;						// keyboard events handler
			Containers::Vector<ManagedResources::ManagedTexturePtr>	m_textures;						// preload all gui-related texture
			ManagedResources::ManagedFontPtr						m_font;							// preload gui font
			Color													m_default_text_color;			// default text color to set to all labels and textboxes
			Color													m_default_frames_color;			// default text color to set to all frames background
			unsigned int											m_font_size;					// font sizes
			Sizei													m_padding;						// padding around gui widgets (empty space around them)
			Color													m_default_text_shadow;			// default shadow color for text
			Pointi													m_default_text_shadow_offset;	// default distance of text shadow

		public:
			// create the gui manager
			// renderer: pointer to the owner renderer
			// resources_path:	the path to the folder that will contain all the gui textures, resources and settings.dat file. 
			//					there is default gui skin provided within ness-engine resources.
			NESSENGINE_API GuiManager(Renderer* renderer, const String& resources_path = "ness-engine/resources/gui/");

			// handle events
			NESSENGINE_API virtual bool inject_event(const Event& event);

			// create a gui container
			// size_in_units is the size of the container in grid_unit_size (as defined when you created the gui manager)
			NESSENGINE_API ContainerPtr create_container(const Pointi& size_in_units);

			// get the renderer
			NESSENGINE_API inline Renderer* renderer() const {return m_renderer;}

			// return the loaded gui font
			NESSENGINE_API inline ManagedResources::ManagedFontPtr get_font() {return m_font;}

			// get the unit size
			NESSENGINE_API inline const Sizei& get_unit_size() const {return m_unit_size;}

			// get padding size
			NESSENGINE_API inline const Sizei& get_padding_size() const {return m_padding;}

			// get the resources path
			NESSENGINE_API inline const String& get_resources_path() const {return m_resources_path;}

			// set/get the default text color
			NESSENGINE_API inline void set_default_text_color(const Color& color) {m_default_text_color = color;}
			NESSENGINE_API inline const Color& get_default_text_color() const {return m_default_text_color;}

			// set/get default text shadow color
			NESSENGINE_API inline void set_default_text_shadow_color(const Color& color) {m_default_text_shadow = color;}
			NESSENGINE_API inline const Color& get_default_text_shadow_color() const {return m_default_text_shadow;}

			// set/get default text shadow distance
			NESSENGINE_API inline void set_default_text_shadow_offset(const Pointi& offset) {m_default_text_shadow_offset = offset;}
			NESSENGINE_API inline const Pointi& get_default_text_shadow_offset() const {return m_default_text_shadow_offset;}

			// set/get the default frames color
			NESSENGINE_API inline void set_default_frames_color(const Color& color) {m_default_frames_color = color;}
			NESSENGINE_API inline const Color& get_default_frames_color() const {return m_default_frames_color;}

			// render the gui!
			NESSENGINE_API void render();

		private:
			// load gui settings from settings file
			NESSENGINE_API void load_settings();

		};

		// a pointer to a gui node
		NESSENGINE_API typedef SharedPtr<GuiManager> GuiManagerPtr;
	}
}