/**
* Our main renderer class!
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../renderable/renderable.h"
#include "../resources/resources_manager.h"
#include "../primitives/primitives.h"
#include "../scene/scene.h"

namespace Ness
{
	// all possible renderer flags
	// this just wraps SDL original flags, so the user won't be confused by mixing names
	enum ERendererFlags
	{
		RENDERER_FLAG_ACCELERATED = SDL_RENDERER_ACCELERATED,
		RENDERER_FLAG_SOFTWARE = SDL_RENDERER_SOFTWARE,
		RENDERER_FLAG_VSYNC = SDL_RENDERER_PRESENTVSYNC,
		RENDERER_FLAG_TARGET_TEXTURE = SDL_RENDERER_TARGETTEXTURE,
	};

	// our main renderer class! manage everything
	class Renderer
	{
	private:
		SDL_Window*								m_window;					// our window pointer
		SDL_Renderer*							m_renderer;					// our main renderer
		ManagedResources::ResourcesManager		m_resources;				// the resources manager class
		std::list<ScenePtr>						m_scenes;					// all the scenes this renderer has
		unsigned int							m_start_frame_time;			// tick count at the begining of the frame
		float									m_timefactor;				// time delta (time factor) from begining to end of frame
		float									m_second_timer;				// count time elapse until getting to a second (0 to 1.0)
		int										m_curr_fps_count;			// count fps
		int										m_fps;						// final fps, updates every second
		Sizei									m_screen_size;				// screen size/ resolution
		bool									m_can_render_to_texture;	// does our renderer support render to texture target?
		ManagedResources::ManagedTexturePtr		m_render_target;			// current texture we render on
		unsigned int							m_frameid;					// a unique frame id, increased by 1 after every frame
		const Sizei*							m_target_size;				// size of the target we are currently rendering on (screen or target texture)

	public:
		// create the renderer instance!
		NESSENGINE_API Renderer(const char* windowName, const Sizei& screenSize, bool FullScreen = false, int rendererFlags = RENDERER_FLAG_ACCELERATED);

		// delete the renderer
		NESSENGINE_API ~Renderer();

		// get time factor for animation calculations
		NESSENGINE_API inline float time_factor() const {return m_timefactor;}

		// get fps count
		NESSENGINE_API inline int fps() const {return m_fps;}

		// set the title of the window
		NESSENGINE_API void set_window_title(const char* NewTitle);

		// get screen size
		NESSENGINE_API const Sizei& get_screen_size() const {return m_screen_size;}

		// get current target size (might be screen size or texture we render on
		NESSENGINE_API const Sizei& get_target_size() const {return *m_target_size;}

		// get the center of the target/screen you currently render to
		NESSENGINE_API Sizei get_target_center() const;
		NESSENGINE_API Sizei get_screen_center() const;

		// create a camera
		NESSENGINE_API CameraPtr create_camera() const {return std::make_shared<Camera>();}

		// return the resources manager
		NESSENGINE_API ManagedResources::ResourcesManager& resources() {return m_resources;}

		// create a new scene
		NESSENGINE_API ScenePtr create_scene();

		// change the background color
		NESSENGINE_API void set_background_color(const Color& NewColor);

		// render everything!
		NESSENGINE_API void render_scenes();

		// begin a rendering frame
		NESSENGINE_API void start_frame(bool clearScene = true);

		// end a rendering frame
		NESSENGINE_API void end_frame();

		// set rendering target. if no texture is provided, will render on the renderer default target (screen)
		NESSENGINE_API void set_render_target(ManagedResources::ManagedTexturePtr texture);
		NESSENGINE_API void reset_render_target();

		// return last renderer error
		NESSENGINE_API inline const char* get_last_renderer_error() const {return SDL_GetError();}

		// clear texture (remove everything)
		NESSENGINE_API void clear_texture(ManagedResources::ManagedTexturePtr texture);

		// return a unique frame id number (increased by 1 every end of frame)
		NESSENGINE_API unsigned int get_frameid() const {return m_frameid;}

		// render surface
		NESSENGINE_API void blit(ManagedResources::ManagedTexturePtr texture, const Rectangle& SrcRect, 
			const Rectangle& TargetRect, EBlendModes mode, Color color = Color::WHITE, float rotation = 0.0f, 
			Point rotation_anchor = Point::HALF);
	};
};