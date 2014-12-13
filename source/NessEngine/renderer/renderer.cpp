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

#include "renderer.h"
#include "../exceptions/exceptions.h"
#include "../scene/scene.h"
#include <algorithm>

// warning C4355: 'this' : used in base member initializer list (disabled due to Animators::AnimatorsQueue(this))
#pragma warning(disable:4355)

namespace Ness
{
	// create the renderer
	Renderer::Renderer(const char* windowName, const Sizei& windowSize, int windowFlags, int rendererFlags) :
		Animators::AnimatorsQueue(this), m_second_timer(0), m_total_time(0), m_curr_fps_count(0), m_fps(0), m_timefactor(0), 
		m_renderer_size(windowSize), m_window_size(windowSize), m_frameid(0), m_background_color(75, 0, 255, 255), m_flags(rendererFlags), 
		m_auto_animate(true)
	{
		base_init();

		// create window
		m_window = SDL_CreateWindow( windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
									(int)m_renderer_size.x, (int)m_renderer_size.y, windowFlags);
		if (m_window == nullptr)
		{
			throw FailedToInitRenderer(SDL_GetError());
		}

		// create our renderer
		m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);

		// give the renderer to the resources manager
		m_resources->set_renderer(this);

		// get the actual window size (if maximized or fullscreen its different then windowSize
		refresh_window_size();

		// to set target size etc..
		reset_render_target();
	}

	// create the renderer from existing window
	Renderer::Renderer(Uint32 window_id, int rendererFlags) :
		Animators::AnimatorsQueue(this), m_flags(rendererFlags)
	{

		base_init();

		// create window
		m_window = SDL_GetWindowFromID(window_id);
		if (m_window == nullptr)
		{
			throw FailedToInitRenderer(SDL_GetError());
		}

		// create our renderer
		m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);

		// get the window size
		refresh_window_size();

		// give the renderer to the resources manager
		m_resources->set_renderer(this);

		// to set target size etc..
		reset_render_target();
	}

	// update window size (call this if window size changes!)
	void Renderer::refresh_window_size()
	{
		SDL_GetWindowSize(m_window, &m_window_size.x, &m_window_size.y);
		if (!m_diff_renderer_size)
			m_renderer_size = m_window_size;
	}

	void Renderer::base_init()
	{
		// set defaults
		m_diff_renderer_size = false;
		m_second_timer = 0;
		m_total_time = 0;
		m_curr_fps_count = 0;
		m_fps = 0;
		m_timefactor = 0;
		m_frameid = 0;
		m_background_color = Ness::Colorb(75, 0, 255, 255);
		m_auto_animate = true;

		// set render to texture flag
		m_can_render_to_texture = ((m_flags & RENDERER_FLAG_TARGET_TEXTURE) != 0);

		// create resources manager
		m_resources = new ManagedResources::ResourcesManager();
	}

	void Renderer::set_renderer_size(const Ness::Sizei& newSize)
	{
		if (newSize != Sizei::ZERO)
		{
			m_renderer_size = newSize;
			SDL_RenderSetLogicalSize(m_renderer, newSize.x, newSize.y);
		}
		else
		{
			m_diff_renderer_size = false;
			refresh_window_size();
		}
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
		return Sizei((int)(m_renderer_size.x * 0.5f), (int)(m_renderer_size.y * 0.5f));
	}

	// create the camera object
	CameraPtr Renderer::create_camera() const 
	{
		return ness_make_ptr<Camera>((Renderer*)this);
	}

	// destroy the renderer
	Renderer::~Renderer()
	{
		// this is very important! its to make sure all sprites are clear and thus all resources are cleared before
		// destroying this window
		m_scenes.clear();

		// destroy all resources in resources manager.
		// better happen before destroying the renderer and window
		m_resources->destroy();

		// destroy renderer and window
		SDL_DestroyRenderer( m_renderer );
		SDL_DestroyWindow( m_window );

		// delete the resources manager
		delete m_resources;
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
		// do animations
		if (m_auto_animate)
			do_animations();

		// render everything and set time factor
		SDL_RenderPresent(m_renderer);
		m_timefactor = (SDL_GetTicks() - m_start_frame_time) / 1000.0f;
		m_total_time += m_timefactor;
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

		// increase frame unique id
		m_frameid++;
	}

	// create a new gui manager
	Gui::GuiManagerPtr Renderer::create_gui_manager(const String& resources_path, const Point& grid_unit_size)
	{
		return ness_make_ptr<Gui::GuiManager>(this, resources_path, grid_unit_size);
	}

	// remove a scene
	void Renderer::remove_scene(const ScenePtr& scene)
	{
		m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene), m_scenes.end());
	}

	// create a new scene
	ScenePtr Renderer::create_scene()
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
		m_target_size = &m_renderer_size;
	}

	void Renderer::push_render_target(const ManagedResources::ManagedTexturePtr& texture)
	{
		// push texture to render targets queue and set as current render target
		m_render_targets_queue.push_back(texture);
		set_render_target(texture);
	}

	// remove all render targets from queue
	void Renderer::clear_render_targets()
	{
		m_render_targets_queue.clear();
		reset_render_target();
	}

	void Renderer::pop_render_target()
	{
		// if not empty, pop last target
		if (!m_render_targets_queue.empty())
			m_render_targets_queue.pop_back();

		// if empty, reset render target
		if (m_render_targets_queue.empty())
			reset_render_target();
		// if not empty, set render target to be next in queue
		else
			set_render_target(m_render_targets_queue.back());
	}

	void Renderer::set_render_target(const ManagedResources::ManagedTexturePtr& texture)
	{
		// got null? remove target texture
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

	void Renderer::clear_texture(ManagedResources::ManagedTexturePtr texture)
	{
		SDL_SetRenderTarget(m_renderer, texture->texture());
		SDL_RenderClear(m_renderer);
		set_render_target(m_render_target);
	}

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
			center.x = (int)floor(rotation_anchor.x * target.w);
			center.y = (int)floor(rotation_anchor.y * target.h);

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