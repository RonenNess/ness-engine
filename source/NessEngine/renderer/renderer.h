/**
* Our main renderer class!
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../managed_resources/resources_manager.h"
#include "../basic_types/all_basic_types.h"
#include "../animators/animator_api.h"
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
		RENDERER_FLAG_LIGHTING_NODE = SDL_RENDERER_TARGETTEXTURE,
	};

	// our main renderer class! manage everything
	class Renderer
	{
	private:
		SDL_Window*									m_window;					// our window pointer
		SDL_Renderer*								m_renderer;					// our main renderer
		ManagedResources::ResourcesManager			m_resources;				// the resources manager class
		Vector<ScenePtr>							m_scenes;					// all the scenes this renderer has
		Vector<Animators::AnimatorPtr>				m_animators;				// all the animators currently registered
		unsigned int								m_start_frame_time;			// tick count at the begining of the frame
		float										m_timefactor;				// time delta (time factor) from begining to end of frame
		float										m_second_timer;				// count time elapse until getting to a second (0 to 1.0)
		float										m_total_time;				// total time passed (1.0f = second)
		int											m_curr_fps_count;			// count fps
		int											m_fps;						// final fps, updates every second
		Sizei										m_screen_size;				// screen size/ resolution
		bool										m_can_render_to_texture;	// does our renderer support render to texture target?
		ManagedResources::ManagedTexturePtr			m_render_target;			// current texture we render on (or null if we render on screen)
		List<ManagedResources::ManagedTexturePtr>	m_render_targets_queue;		// queue of render targets waiting in line
		unsigned int								m_frameid;					// a unique frame id, increased by 1 after every frame
		const Sizei*								m_target_size;				// size of the target we are currently rendering on (screen or target texture)
		Colorb										m_background_color;			// background clear color
		const int									m_flags;					// init flags (passed in constructor)
		bool										m_auto_animate;				// do animations automatically (default to true)

	public:
		// create the renderer instance!
		// windowName - name to give to the main application window (and caption)
		// screenSize - resolution / window size
		// fullScreen - if true will run ness-engine at fullscreen mode
		// rendererFlags - creation flags. check out ERendererFlags for more information
		NESSENGINE_API Renderer(const char* windowName, const Sizei& screenSize, bool fullScreen = false, int rendererFlags = RENDERER_FLAG_ACCELERATED | RENDERER_FLAG_LIGHTING_NODE);

		// delete the renderer
		NESSENGINE_API ~Renderer();

		// return the init flags
		NESSENGINE_API inline int get_flags() const {return m_flags;}

		// get time factor for animation calculations
		NESSENGINE_API inline float time_factor() const {return m_timefactor;}

		// get total time passed since starting rendering and until now (counting only when calling end frame)
		NESSENGINE_API inline float get_total_time_elapse() const {return m_total_time;}

		// register / remove an animator
		NESSENGINE_API void register_animator(const Animators::AnimatorPtr& animator);
		NESSENGINE_API void remove_animator(const Animators::AnimatorPtr& animator);

		// run all the animators
		NESSENGINE_API void do_animations();

		// get fps count
		NESSENGINE_API inline int fps() const {return m_fps;}

		// enable/disable auto animate (default to true)
		NESSENGINE_API inline void animate_automatically(bool enable) {m_auto_animate = enable;}

		// set the title of the window
		NESSENGINE_API void set_window_title(const String& NewTitle);

		// get target/screen size (we are currently rendering to)
		NESSENGINE_API const Sizei& get_screen_size() const {return m_screen_size;}
		NESSENGINE_API const Sizei& get_target_size() const {return *m_target_size;}

		// get the center of the target/screen you currently render to
		NESSENGINE_API Sizei get_target_center() const;
		NESSENGINE_API Sizei get_screen_center() const;

		// create a camera
		NESSENGINE_API CameraPtr create_camera() const {return ness_make_ptr<Camera>();}

		// return the resources manager
		NESSENGINE_API ManagedResources::ResourcesManager& resources() {return m_resources;}

		// create/remove a scene
		NESSENGINE_API ScenePtr create_scene();
		NESSENGINE_API void remove_scene(const ScenePtr& scene);

		// change the background color
		NESSENGINE_API void set_background_color(const Color& NewColor);

		// render everything!
		NESSENGINE_API void render_scenes(const CameraPtr& camera = NullCamera);

		// begin a rendering frame
		NESSENGINE_API void start_frame(bool clearScene = true);

		// end a rendering frame
		NESSENGINE_API void end_frame();

		// push render target (texture) to the render targets queue. the renderer will render everything on the target at the top of the queue
		// so this will basically set the current rendering target. after you finish pop the rendering target with pop_render_target();
		NESSENGINE_API void push_render_target(const ManagedResources::ManagedTexturePtr& texture);
		NESSENGINE_API void pop_render_target();

		// remove all render targets from queue
		NESSENGINE_API void clear_render_targets();

		// return last renderer error
		NESSENGINE_API inline const char* get_last_renderer_error() const {return SDL_GetError();}

		// clear texture (remove everything)
		NESSENGINE_API void clear_texture(ManagedResources::ManagedTexturePtr texture);

		// fill texture to given color
		NESSENGINE_API void fill_texture(ManagedResources::ManagedTexturePtr texture, const Color& fillColor);

		// return a unique frame id number (increased by 1 every end of frame)
		NESSENGINE_API unsigned int get_frameid() const {return m_frameid;}

		// render managed texture
		NESSENGINE_API inline void blit(ManagedResources::ManagedTexturePtr texture, const Rectangle* SrcRect, 
			const Rectangle& TargetRect, EBlendModes mode = BLEND_MODE_NONE, const Color& color = Color::WHITE, float rotation = 0.0f, 
			Point rotation_anchor = Point::HALF) 
		{
			blit(texture->texture(), SrcRect, TargetRect, mode, color, rotation, rotation_anchor);
		}

		// render raw SDL texture
		NESSENGINE_API void blit(SDL_Texture* texture, const Rectangle* SrcRect, 
			const Rectangle& TargetRect, EBlendModes mode = BLEND_MODE_NONE, const Color& color = Color::WHITE, float rotation = 0.0f, 
			Point rotation_anchor = Point::HALF);

		// draw rectagnle
		NESSENGINE_API void draw_rect(const Rectangle& TargetRect, const Color& color, bool filled = true, EBlendModes mode = BLEND_MODE_NONE);

		// return the sdl renderer
		inline SDL_Renderer* __sdl_renderer() {return m_renderer;}

	protected:
		// set/remove the current rendering target. note: this does not effect the rendering targets queue, it only set or reset the current target
		NESSENGINE_API void set_render_target(const ManagedResources::ManagedTexturePtr& texture);
		NESSENGINE_API void reset_render_target();

	};
};