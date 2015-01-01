#include "gui_manager.h"
#include "../renderer/renderer.h"

namespace Ness
{
	namespace Gui
	{
			GuiManager::GuiManager(Renderer* renderer, const String& resources_path, const Pointi& grid_unit_size, const int font_size)
		: m_renderer(renderer), m_unit_size(grid_unit_size), m_resources_path(resources_path + "/") 
		{
			// create the root container
			m_root_container = ness_make_ptr<RootContainer>(this);

			// pre-load gui font
			m_font = m_renderer->resources().get_font(m_resources_path + "font.ttf", font_size);

			// set default text color
			set_default_text_color(Color::BLACK);

			// pre-load all gui textures
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "frame_disabled.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "frame_focused.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "frame_unfocused.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "frame_mouse_hover.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "frame_mouse_down.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "button_disabled.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "button_focused.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "button_unfocused.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "button_mouse_hover.png"));
			m_textures.push_back(m_renderer->resources().get_texture(m_resources_path + "button_mouse_down.png"));
		}

		ContainerPtr GuiManager::create_container(const Pointi& size_in_units)
		{
			return m_root_container->create_container(size_in_units);
		}

		void GuiManager::render()
		{
			m_root_container->get_node()->render();
		}

		bool GuiManager::inject_event(const Event& event)
		{

			bool ret = false;

			// check the type of event called
			switch (event.type)
			{
				// do mouse movement
				case SDL_MOUSEMOTION:
					ret = m_root_container->handle_mouse_move(m_mouse.position());
					break;

				case SDL_MOUSEBUTTONDOWN:
					ret = m_root_container->handle_mouse_state((EMouseButtons)event.button.button, true, m_mouse.position());
					break;

				case SDL_MOUSEBUTTONUP:
					ret = m_root_container->handle_mouse_state((EMouseButtons)event.button.button, false, m_mouse.position());
					break;
			}

			// update mouse and keyboard
			// we use this to remember the last keyboard and mouse state
			m_mouse.inject_event(event);
			m_keyboard.inject_event(event);

			return ret;
		}
	}
}