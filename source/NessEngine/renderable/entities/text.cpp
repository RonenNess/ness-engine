#include "text.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	
	Text::Text(Renderer* renderer, NodeAPI* parent, ManagedResources::ManagedFontPtr font, const std::string& text) : 
		Entity(renderer, parent), m_need_text_update(true), m_texture(nullptr)
	{
		change_font(font);
		change_text(text);
		set_static(true);
		set_size(Size(100, 100));	// <-- must call this or 'update' will never be called
		set_blend_mode(BLEND_MODE_BLEND);
	}

	Text::Text(Renderer* renderer, NodeAPI* parent, const std::string& FontFile, const std::string& text, int fontSize) : 
		Entity(renderer, parent), m_need_text_update(true), m_texture(nullptr)
	{
		change_font(m_renderer->resources().get_font(FontFile, fontSize));
		change_text(text);
		set_static(true);
		set_size(Size(100, 100));	// <-- must call this or 'update' will never be called
		set_blend_mode(BLEND_MODE_BLEND);
	}

	void Text::update()
	{
		// free previous texture if exist
		if (m_texture)
		{
			SDL_DestroyTexture(m_texture);
			m_texture = nullptr;
		}

		// create the texture for this text
		static SDL_Color text_color = {255, 255, 255};
		SDL_Surface* surface = TTF_RenderText_Solid(m_font->font(), m_text.c_str(), text_color);
		m_texture = SDL_CreateTextureFromSurface(m_renderer->__sdl_renderer(), surface);
		SDL_FreeSurface(surface);

		// get its size for default size
		Uint32 format;
		int access;
		Sizei size;
		SDL_QueryTexture(m_texture, &format, &access, &size.x, &size.y);
		set_size(size);

		// no longer need update
		m_need_text_update = false;
	}

	void Text::do_render(const Rectangle& target, const SRenderTransformations& transformations)
	{
		// if need to update the surface call update
		if (m_need_text_update)
		{
			update();
		}

		m_renderer->blit(m_texture, nullptr, target, transformations.blend, transformations.color, transformations.rotation, m_anchor);
	}
};