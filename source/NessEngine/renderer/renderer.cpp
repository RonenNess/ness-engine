#include "renderer.h"
#include "../exceptions/exceptions.h"
#include "../scene/scene.h"
#include <algorithm>

namespace Ness
{
	// create the renderer
	Renderer::Renderer(const char* windowName, const Sizei& screenSize, bool FullScreen, int rendererFlags) :
		m_second_timer(0), m_curr_fps_count(0), m_fps(0), m_timefactor(0), m_screen_size(screenSize), 
		m_frameid(0), m_background_color(75, 0, 255, 255), m_flags(rendererFlags), m_auto_animate(true)
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
	void Renderer::set_window_title(const String& NewTitle)
	{
		SDL_SetWindowTitle(m_window, NewTitle.c_str());
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
		// begin scene and clear if needed
		m_start_frame_time = SDL_GetTicks();
		if (clearScene) 
		{
			SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
			SDL_SetRenderDrawColor(m_renderer, (m_background_color.r), (m_background_color.g), (m_background_color.b), m_background_color.a);
			SDL_RenderClear(m_renderer);
		}
	}

	// end a rendering frame
	void Renderer::end_frame()
	{
		// render everything and set time factor
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

		// do animations
		if (m_auto_animate)
			do_animations();

		// increase frame unique id
		m_frameid++;
	}

	// register an animator
	void Renderer::register_animator(const Animators::AnimatorPtr& animator)
	{
		m_animators.push_back(animator);
	}

	// remove an animator
	void Renderer::remove_animator(const Animators::AnimatorPtr& animator)
	{
		m_animators.erase(std::remove(m_animators.begin(), m_animators.end(), animator), m_animators.end());
	}

	// remove all dead animators from list
	bool remove_dead_animators(const Animators::AnimatorPtr& animator)
	{
		return animator->__should_be_removed();
	}

	// run all animators
	void Renderer::do_animations()
	{
		// loop and animate all animators
		bool gotAnimatorsToRemove = false;
		for (unsigned int i = 0; i < m_animators.size(); ++i)
		{
			Animators::AnimatorPtr& curr = m_animators[i];
			if (curr->is_animation_paused())
				continue;

			curr->do_animation(this);
			gotAnimatorsToRemove |= curr->__should_be_removed();
		}

		// remove all 'dead' animators
		if (gotAnimatorsToRemove)
			m_animators.erase(std::remove_if(m_animators.begin(), m_animators.end(), remove_dead_animators), m_animators.end());
	}

	// remove a scene
	NESSENGINE_API void Renderer::remove_scene(const ScenePtr& scene)
	{
		m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene), m_scenes.end());
	}

	// create a new scene
	NESSENGINE_API ScenePtr Renderer::create_scene()
	{
		ScenePtr ret = ScenePtr(new Scene(this));
		m_scenes.push_back(ret);
		return ret;
	}

	// render everything!
	void Renderer::render_scenes(const CameraPtr& camera)
	{
		// render everything
		for (auto scene = m_scenes.begin(); scene != m_scenes.end(); ++scene)
		{
			(*scene)->render(camera);
		}	
	}

	// set background color
	void Renderer::set_background_color(const Color& NewColor)
	{
		m_background_color.r = (unsigned char)(NewColor.r * 255);
		m_background_color.g = (unsigned char)(NewColor.g * 255);
		m_background_color.b = (unsigned char)(NewColor.b * 255);
		m_background_color.a = (unsigned char)(NewColor.a * 255);
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

	// fill texture to given color
	void Renderer::fill_texture(ManagedResources::ManagedTexturePtr texture, const Color& fillColor)
	{
		Rectangle screen;
		screen.x = 0;
		screen.y = 0;
		screen.w = texture->get_size().x;
		screen.h = texture->get_size().y;
		SDL_SetRenderTarget(m_renderer, texture->texture());
		draw_rect(screen, fillColor, true);
		set_render_target(m_render_target);
	}

	void Renderer::draw_rect(const Rectangle& TargetRect, const Color& color, bool filled, EBlendModes mode)
	{
		// check if in screen
		if (TargetRect.x >= m_target_size->x || TargetRect.y >= m_target_size->y || TargetRect.x + TargetRect.w <= 0 || TargetRect.y + TargetRect.h <= 0 )
		{
			return;
		}

		// set blend mode and color
		SDL_SetRenderDrawBlendMode(m_renderer, (SDL_BlendMode)mode);
		SDL_SetRenderDrawColor(m_renderer, (Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 255));

		// render filled/hollow rectangle
		if (filled)
		{
			SDL_RenderFillRect(m_renderer, &TargetRect);
		}
		else
		{
			SDL_RenderDrawRect(m_renderer, &TargetRect);
		}
	}

	// render surface
	void Renderer::blit(SDL_Texture* texture, const Rectangle* SrcRect, const Rectangle& TargetRect, EBlendModes mode, const Color& color, float rotation, Point rotation_anchor)
	{

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

		// check if in screen
		if (target.x >= m_target_size->x || target.y >= m_target_size->y || target.x + target.w <= 0 || target.y + target.h <= 0 )
		{
			return;
		}

		// set alpha
		float alpha = color.a;
		SDL_SetTextureAlphaMod(texture, (int)(alpha * 255));

		// set color
		SDL_SetTextureColorMod(texture, (Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255));

		// set blend mode
		SDL_SetTextureBlendMode(texture, (SDL_BlendMode)mode);

		// requires advance rendering?
		if (flip || (rotation != 0.0f) || (alpha < 1.0f))
		{

			// set rotation anchor
			SDL_Point center;
			center.x = (int)(rotation_anchor.x * target.w);
			center.y = (int)(rotation_anchor.y * target.h);

			// render with full settings!
			SDL_RenderCopyEx(m_renderer, texture, SrcRect, &target, rotation, &center, (SDL_RendererFlip)flip);
		}
		// simple rendering - no flip, no rotation
		else
		{
			SDL_RenderCopy(m_renderer, texture, SrcRect, &target);
		}
	}
};