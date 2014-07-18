#include "text.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	
	Text::Text(Renderer* renderer, ManagedResources::ManagedFontPtr font, const String& text) : 
		Entity(renderer), m_need_text_update(true), m_texture(nullptr), m_line_width(0)
	{
		change_font(font);
		set_size(Size(100, 100));	// <-- must call this or 'update' will never be called
		change_text(text);
		set_static(true);
		set_blend_mode(BLEND_MODE_BLEND);
	}

	Text::Text(Renderer* renderer, const String& FontFile, const String& text, int fontSize) : 
		Entity(renderer), m_need_text_update(true), m_texture(nullptr), m_line_width(0)
	{
		change_font(m_renderer->resources().get_font(FontFile, fontSize));
		set_size(Size(100, 100));	// <-- must call this or 'update' will never be called
		change_text(text);
		set_static(true);
		set_blend_mode(BLEND_MODE_BLEND);
	}

	void Text::set_alignment(ETextAlignment align)
	{
		switch (align)
		{
		case TEXT_ALIGN_LEFT:
			m_anchor.x = 0.0f;
			break;

		case TEXT_ALIGN_RIGHT:
			m_anchor.x = 1.0f;
			break;

		case TEXT_ALIGN_CENTER:
			m_anchor.x = 0.5f;
			break;
		};
		transformations_update();
	}

	void Text::update()
	{
		// free previous texture if exist
		if (m_texture)
		{
			SDL_DestroyTexture(m_texture);
			m_texture = nullptr;
		}

		// render text on surface
		static SDL_Color text_color = {255, 255, 255};
		SDL_Surface* surface = nullptr;
		if (m_line_width > 0)
		{
			surface = TTF_RenderText_Blended_Wrapped(m_font->font(), m_text.c_str(), text_color, m_line_width);
		}
		else
		{
			surface = TTF_RenderText_Solid(m_font->font(), m_text.c_str(), text_color);
		}
		if (surface == nullptr)
		{
			NESS_ERROR((String("failed to create text surface: ") + TTF_GetError()).c_str());
			return;
		}
		
		// convert to texture
		m_texture = SDL_CreateTextureFromSurface(m_renderer->__sdl_renderer(), surface);
		SDL_FreeSurface(surface);

		if (m_texture == nullptr)
		{
			NESS_ERROR((String("failed to create text texture: ") + TTF_GetError()).c_str());
			return;
		}

		// get its size for default size
		Uint32 format;
		int access;
		Sizei size;
		SDL_QueryTexture(m_texture, &format, &access, &size.x, &size.y);
		set_size(size);

		// no longer need update text, but need to update transformations
		transformations_update();
		m_need_text_update = false;
	}

	void Text::render(const CameraPtr& camera)
	{
		// if need to update the text surface call update
		if (m_need_text_update)
		{
			update();
		}

		// call the base render function
		Entity::render(camera);
	}

	void Text::do_render(const Rectangle& target, const SRenderTransformations& transformations)
	{
		m_renderer->blit(m_texture, nullptr, target, transformations.blend, transformations.color, transformations.rotation, m_anchor);
	}
};