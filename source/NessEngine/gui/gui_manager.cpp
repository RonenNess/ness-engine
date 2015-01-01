#include "gui_manager.h"
#include "../renderer/renderer.h"
#include "../exceptions/exceptions.h"
#include <iostream>
#include <fstream>

namespace Ness
{
	namespace Gui
	{
		GuiManager::GuiManager(Renderer* renderer, const String& resources_path)
			: m_renderer(renderer), m_resources_path(resources_path + "/") 
		{
			// create the root container
			m_root_container = ness_make_ptr<RootContainer>(this);

			// set defaults
			m_unit_size = Point(32, 32);
			m_default_text_color = Color::BLACK;
			m_default_frames_color = Color::WHITE;
			m_font_size = 17;
			m_default_text_shadow = Color(0.0f, 0.0f, 0.0f, 0.5f);
			m_default_text_shadow_offset = Point(1, 1);

			// load gui settings file
			load_settings();

			// pre-load gui font
			m_font = m_renderer->resources().get_font(m_resources_path + "font.ttf", m_font_size * 2);

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

		void GuiManager::load_settings()
		{

			// open data file
			std::ifstream infile;
			infile.open(m_resources_path + "settings.dat");
			if (infile.bad() || infile.eof()) 
			{
				throw FileNotFound((m_resources_path + "settings.dat").c_str());
			}

			// load all gui settings
			std::string line;
			while (std::getline(infile, line))
			{
				// ignore comments and empty lines
				if (line.length() == 0 || line[0] == '#')
					continue;

				// split into param name and value
				std::string param = line.substr(0, line.find(':'));
				std::string value = line.substr(line.find(':') + 1);

				// if there is space after the ':', remove it
				while ((value[0] == ' ' || value[0] == '\t') && value.length() > 0)
				{
					value = value.substr(1);
				}

				// make sure value and param are valid
				if (value.length() == 0 || param.length() == 0)
				{
					throw WrongFormatError(("Invalid key or value loaded in gui settings.dat! line: " + line).c_str());
				}

				// now parse params
				if (param == "default_font_color")
				{
					m_default_text_color.deserialize(value);
				}
				else if (param == "default_frames_color")
				{
					m_default_frames_color.deserialize(value);
				}
				else if (param == "font_size")
				{
					m_font_size = atoi(value.c_str());
				}
				else if (param == "grid_unit_size")
				{
					m_unit_size.deserialize(value);
				}
				else if (param == "padding")
				{
					m_padding.deserialize(value);
				}
				if (param == "default_font_shadow_color")
				{
					m_default_text_shadow.deserialize(value);
				}
				else if (param == "default_font_shadow_offset")
				{
					m_default_text_shadow_offset.deserialize(value);
				}
				else
				{
					throw WrongFormatError(("Unknown parameter in gui settings.dat! line: " + line).c_str());
				}
			}

			// fix some defaults
			if (m_padding == Sizei::ZERO)
				m_padding = m_unit_size / 2;
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