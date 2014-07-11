#include "renderer.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	// create the renderer
	Renderer::Renderer(const char* windowName, const Sizei& screenSize, bool FullScreen, int rendererFlags) :
		m_second_timer(0), m_curr_fps_count(0), m_fps(0), m_timefactor(0), m_screen_size(screenSize), m_frameid(0)
	{

		// create window
		m_window = SDL_CreateWindow( windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
									(int)screenSize.x, (int)screenSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | ( FullScreen ? SDL_WINDOW_FULLSCREEN : 0) );
		if (m_window == nullptr)
		{
			throw FailedToInitRenderer(SDL_GetError());
		}

		// set render to texture flag
		m_can_render_to_texture = ((rendererFlags & RENDERER_FLAG_TARGET_TEXTURE) != 0);

		// create our renderer
		m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);

		// give the renderer to the resources manager
		m_resources.set_renderer(m_renderer);

		// to set target size etc..
		reset_render_target();
	}

	// set the window title
	void Renderer::set_window_title(const char* NewTitle)
	{
		SDL_SetWindowTitle(m_window, NewTitle);
	}

	// create a new scene
	ScenePtr Renderer::create_scene()
	{
		ScenePtr ret = ScenePtr(new Scene(this));
		m_scenes.push_back(ret);
		return ret;
	}

	// get center of current render target
	Sizei Renderer::get_target_center() const
	{
		return Sizei((int)(m_target_size->x * 0.5f), (int)(m_target_size->y * 0.5f));
	}

	// get center of screen
	Sizei Renderer::get_screen_center() const
	{
		return Sizei((int)(m_screen_size.x * 0.5f), (int)(m_screen_size.y * 0.5f));
	}

	// destroy the renderer
	Renderer::~Renderer()
	{
		SDL_DestroyRenderer( m_renderer );
		SDL_DestroyWindow( m_window );
	}

	// begin a rendering frame
	void Renderer::start_frame(bool clearScene)
	{
		m_start_frame_time = SDL_GetTicks();
		if (clearScene) SDL_RenderClear(m_renderer);
	}

	// end a rendering frame
	void Renderer::end_frame()
	{
		SDL_RenderPresent(m_renderer);
		m_timefactor = (SDL_GetTicks() - m_start_frame_time) / 1000.0f;
		m_second_timer += m_timefactor;
		if (m_second_timer >= 1.0f)
		{
			m_second_timer = 0.0f;
			m_fps = m_curr_fps_count;
			m_curr_fps_count = 0;
		}
		else
		{
			m_curr_fps_count++;
		}

		m_frameid++;
	}

	// render everything!
	void Renderer::render_scenes()
	{
		// render everything
		for (auto scene = m_scenes.begin(); scene != m_scenes.end(); ++scene)
		{
			(*scene)->render();
		}	
	}

	// set background color
	void Renderer::set_background_color(const Color& NewColor)
	{
		SDL_SetRenderDrawColor(m_renderer, (int)(NewColor.r * 255), (int)(NewColor.g * 255), (int)(NewColor.b * 255), 255);
	}

	void Renderer::reset_render_target()
	{
		SDL_SetRenderTarget(m_renderer, nullptr); 
		m_render_target.reset(); 
		m_target_size = &m_screen_size;
	}

	// set rendering to target texture
	void Renderer::set_render_target(ManagedResources::ManagedTexturePtr texture)
	{
		// got null: remove target texture
		if (texture.get() == nullptr)
		{
			reset_render_target();
			return;
		}

		// make sure we can render to texture
		if (!m_can_render_to_texture) throw IllegalAction("Cannot render to texture without setting the RENDERER_FLAG_TARGET_TEXTURE renderer flag!");

		// set target texture
		SDL_SetRenderTarget(m_renderer, texture->texture());
		m_render_target = texture;
		m_target_size = &texture->get_size();
	}

	// clear texture (remove everything)
	void Renderer::clear_texture(ManagedResources::ManagedTexturePtr texture)
	{
		SDL_SetRenderTarget(m_renderer, texture->texture());
		SDL_RenderClear(m_renderer);
		set_render_target(m_render_target);
	}

	// render surface
	void Renderer::blit(ManagedResources::ManagedTexturePtr texture, const Rectangle& SrcRect, const Rectangle& TargetRect, EBlendModes mode, Color color, float rotation, Point rotation_anchor)
	{
		// check if in screen
		if (TargetRect.x >= m_target_size->x || TargetRect.y >= m_target_size->y || TargetRect.x + TargetRect.w <= 0 || TargetRect.y + TargetRect.h <= 0 )
		{
			return;
		}

		// set flipping for negative scale and fix target size
		Rectangle target = TargetRect;
		int flip = SDL_FLIP_NONE;
		if (target.w < 0)
		{
			target.w *= -1;
			flip |= SDL_FLIP_HORIZONTAL;
		}
		if (target.h < 0)
		{
			target.h *= -1;
			flip |= SDL_FLIP_VERTICAL;
		}

		// set alpha
		float alpha = color.a;
		SDL_SetTextureAlphaMod(texture->texture(), (int)(alpha * 255));

		// set color
		SDL_SetTextureColorMod(texture->texture(), (Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255));

		// set blend mode
		SDL_SetTextureBlendMode(texture->texture(), (SDL_BlendMode)mode);

		// requires advance rendering?
		if (flip || (rotation != 0.0f) || (alpha < 1.0f))
		{

			// set rotation anchor
			SDL_Point center;
			center.x = (int)(rotation_anchor.x * target.w);
			center.y = (int)(rotation_anchor.y * target.h);

			// render with full settings!
			SDL_RenderCopyEx(m_renderer, texture->texture(), &SrcRect, &target, rotation, &center, (SDL_RendererFlip)flip);
		}
		// simple rendering - no flip, no rotation
		else
		{
			SDL_RenderCopy(m_renderer, texture->texture(), &SrcRect, &target);
		}
	}
};