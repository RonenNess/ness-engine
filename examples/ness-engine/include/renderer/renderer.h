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
* Our main renderer class! this manage the rendering functionality, the begining and ending of frames,
* time factor, and animator queues. we usually create only one instance of a renderer and use it
* throught-out the entire project.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../managed_resources/resources_manager.h"
#include "../basic_types/all_basic_types.h"
#include "../animators/animators_queue.h"
#include "../scene/scene.h"
#include "../gui/gui_manager.h"

namespace Ness
{
	// all possible renderer flags (flags about what the rendering device will support)
	// this just wraps SDL original flags, so the user won't be confused by mixing names
	enum ERendererFlags
	{
		RENDERER_FLAG_ACCELERATED = SDL_RENDERER_ACCELERATED,
		RENDERER_FLAG_SOFTWARE = SDL_RENDERER_SOFTWARE,
		RENDERER_FLAG_VSYNC = SDL_RENDERER_PRESENTVSYNC,
		RENDERER_FLAG_TARGET_TEXTURE = SDL_RENDERER_TARGETTEXTURE,
		RENDERER_FLAG_LIGHTING_NODE = SDL_RENDERER_TARGETTEXTURE,
	};

	// all possible window creation flags (flags about creating the window)
	// this just wraps SDL original flags, so the user won't be confused by mixing names
	enum EWindowsFlags
	{
		WINDOW_FLAG_FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		WINDOW_FLAG_OPENGL = SDL_WINDOW_OPENGL,
		WINDOW_FLAG_SHOWN = SDL_WINDOW_SHOWN,
		WINDOW_FLAG_HIDDEN = SDL_WINDOW_HIDDEN,
		WINDOW_FLAG_BORDERLESS = SDL_WINDOW_BORDERLESS,
		WINDOW_FLAG_RESIZABLE = SDL_WINDOW_RESIZABLE,
		WINDOW_FLAG_MINIMIZED = SDL_WINDOW_MINIMIZED,
		WINDOW_FLAG_MAXIMIZED = SDL_WINDOW_MAXIMIZED,
		WINDOW_FLAG_INPUT_GRABBED = SDL_WINDOW_INPUT_GRABBED,
		WINDOW_FLAG_INPUT_FOCUS = SDL_WINDOW_INPUT_FOCUS,
		WINDOW_FLAG_MOUSE_FOCUS = SDL_WINDOW_MOUSE_FOCUS,
		WINDOW_FLAG_FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
		WINDOW_FLAG_FOREIGN = SDL_WINDOW_FOREIGN,
		WINDOW_FLAG_ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI
	};

	// default init flags
	enum EDefaultCreationFlags
	{
		DEFAULT_WINDOW_FLAGS =				(WINDOW_FLAG_SHOWN | WINDOW_FLAG_OPENGL),
		DEFAULT_WINDOW_FLAGS_FULLSCREEN =	(DEFAULT_WINDOW_FLAGS | WINDOW_FLAG_FULLSCREEN),
		DEFAULT_RENDERER_FLAGS =			(RENDERER_FLAG_ACCELERATED | RENDERER_FLAG_LIGHTING_NODE)
	};

	/**
	* our main renderer class! manage all the rendering and frames functionality.
	* usually you only create 1 renderer class, but you can also create multiple renderers.
	*/
	class Renderer : public Animators::AnimatorsQueue
	{
	private:
		SDL_Window*													m_window;					// our window pointer
		SDL_Renderer*												m_renderer;					// our main renderer
		ManagedResources::ResourcesManager*							m_resources;				// the resources manager class
		Containers::Vector<ScenePtr>								m_scenes;					// all the scenes this renderer has
		unsigned int												m_start_frame_time;			// tick count at the begining of the frame
		float														m_timefactor;				// time delta (time factor) from begining to end of frame
		float														m_second_timer;				// count time elapse until getting to a second (0 to 1.0)
		float														m_total_time;				// total time passed (1.0f = second)
		int															m_curr_fps_count;			// count fps
		int															m_fps;						// final fps, updates every second
		Sizei														m_renderer_size;			// the actual renderer size (and resolution if fullscreen)
		Sizei														m_window_size;				// the size of the window itself. usually this is the same as m_renderer_size, unless you use set_renderer_size()
		bool														m_can_render_to_texture;	// does our renderer support render to texture target?
		ManagedResources::ManagedTexturePtr							m_render_target;			// current texture we render on (or null if we render on screen)
		Containers::List<ManagedResources::ManagedTexturePtr>		m_render_targets_queue;		// queue of render targets waiting in line
		unsigned int												m_frameid;					// a unique frame id, increased by 1 after every frame
		const Sizei*												m_target_size;				// size of the target we are currently rendering on (screen or target texture)
		Colorb														m_background_color;			// background clear color
		const int													m_flags;					// init flags (passed in constructor)
		bool														m_auto_animate;				// do animations automatically (default to true)
		bool														m_diff_renderer_size;		// are we using different renderer size? (set_renderer_size)

	public:
		// create the renderer instance!
		// windowName - name to give to the main application window (and caption)
		// windowSize - window size, or resolution if in fullscreen mode (if fullscreen and screenSize is 0, will use current resolution)
		// windowFlags - window creation flags, determine how the window will be created and weather or not it will be fullscreen. check out EWindowsFlags for more information.
		// rendererFlags - renderer-related flags, determine some of the renderer capabilities. check out ERendererFlags for more information
		NESSENGINE_API Renderer(const char* windowName, const Sizei& windowSize,
			int windowFlags = DEFAULT_WINDOW_FLAGS, 
			int rendererFlags = DEFAULT_RENDERER_FLAGS);

		// create the renderer instance from an existing window/form!
		// window_id - the id of the window/form to use
		// rendererFlags - renderer-related flags, determine some of the renderer capabilities. check out ERendererFlags for more information
		NESSENGINE_API Renderer(Uint32 id_window, int rendererFlags = DEFAULT_RENDERER_FLAGS);

		// delete the renderer
		NESSENGINE_API ~Renderer();

		// update window size (call this if window size changes!)
		NESSENGINE_API void refresh_window_size();

		// return the multiply factor between the window size and the renderer size.
		// if you didn't use set_renderer_size() this should always be (1,1)
		// if you set alternative renderer size this function will return 
		NESSENGINE_API Size get_renderer_to_window_ratio() const {return (Size(m_renderer_size) / Size(m_window_size));}

		// set the logical size of the renderer
		// for example if you have full resolution of 1600x1200 and renderer size of 640x480, it will automatically scale the
		// rendering to be 640x480-like resolution while really working on 1600x1200 resolution.
		// this is useful if your game is in pixel art and you want low resolution - use this!
		// IMPORTANT!!!! if you want to use mouse position relative to the renderer size (what you would usually want)
		// always multiply the mouse position with get_renderer_to_window_ratio();
		// note: provide ZERO size to cancel renderer size and just use the window size
		NESSENGINE_API void set_renderer_size(const Sizei& newSize);

		// return the init flags
		NESSENGINE_API inline int get_flags() const {return m_flags;}

		// get time factor for animation calculations
		NESSENGINE_API inline float time_factor() const {return m_timefactor;}

		// get total time passed since starting rendering and until now (counting only when calling end frame)
		NESSENGINE_API inline float get_total_time_elapse() const {return m_total_time;}

		// get fps count
		NESSENGINE_API inline int fps() const {return m_fps;}

		// enable/disable auto animate (default to true)
		NESSENGINE_API inline void animate_automatically(bool enable) {m_auto_animate = enable;}

		// set the title of the window
		NESSENGINE_API void set_window_title(const String& NewTitle);

		// get target/screen size (we are currently rendering to)
		NESSENGINE_API const Sizei& get_screen_size() const {return m_renderer_size;}
		NESSENGINE_API const Sizei& get_target_size() const {return *m_target_size;}

		// get the center of the target/screen you currently render to
		NESSENGINE_API Sizei get_target_center() const;
		NESSENGINE_API Sizei get_screen_center() const;

		// create a camera
		NESSENGINE_API CameraPtr create_camera() const;

		// return the resources manager
		NESSENGINE_API ManagedResources::ResourcesManager& resources() {return *m_resources;}

		// create/remove a scene
		NESSENGINE_API ScenePtr create_scene();
		NESSENGINE_API void remove_scene(const ScenePtr& scene);

		// create a gui manager
		// resources_path: the path to the folder that will contain all the gui textures. 
		//					there is default gui skin provided within ness-engine resources.
		// grid_unit_size: all sizes and units in gui elements are based on the grid. this 
		//					param determine the size of a single grid unit.
		NESSENGINE_API Gui::GuiManagerPtr create_gui_manager(const String& resources_path = "ness-engine/resources/gui/", 
				const Point& grid_unit_size = Point(32, 32));

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
		NESSENGINE_API inline unsigned int get_frameid() const {return m_frameid;}

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

	private:
		// set some starting default values
		NESSENGINE_API void base_init();

	};
};