/**
* An object you can render, or a scene node. has rendering properties and render API to implement.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../exports.h"
#include "renderable_parent.h"
#include "../primitives/primitives.h"
#include "../scene/camera.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	// predeclare node and renderer classes
	class Renderer;

	// possible blend modes
	enum EBlendModes
	{
		BLEND_MODE_NONE = SDL_BLENDMODE_NONE,
		BLEND_MODE_BLEND = SDL_BLENDMODE_BLEND,
		BLEND_MODE_ADD = SDL_BLENDMODE_ADD,
		BLEND_MODE_MOD = SDL_BLENDMODE_MOD
	};

	// rendering transformation
	struct SRenderTransformations
	{
		Point				position;
		Size				scale;
		float				rotation;
		EBlendModes			blend;
		Color				color;
		float				zorder;

		NESSENGINE_API SRenderTransformations(Point Position = Size(0, 0), Size Scale = Size(1, 1), float Rotation = 0.0f,  
								EBlendModes Blend = BLEND_MODE_NONE, Color RenderColor = Color::WHITE, float Zorder = 0.0f) :
				position(Position), scale(Scale), rotation(Rotation), blend(Blend), color(RenderColor), zorder(Zorder)
		{
		}

		// combine other transformations into this transformations
		NESSENGINE_API void add_transformations(const SRenderTransformations& other)
		{
			position += other.position;
			scale *= other.scale;
			rotation += other.rotation;
			if ((int)other.blend > (int)blend) blend = other.blend;
			color *= other.color;
		}
	};

	// a renderable object/node
	class Renderable
	{
	protected:
		Renderer*				m_renderer;						// pointer to the renderer
		RenderableParent*		m_parent;						// parent node
		SRenderTransformations	m_transformations;				// render transformations (opacity, size, position, ...)
		bool					m_visible;						// should this renderable be displayed or not

	public:
		NESSENGINE_API Renderable(Renderer* renderer, RenderableParent* parent = nullptr) : 
			m_renderer(renderer), m_parent(parent), m_visible(true) {}

		// setters/getters
		NESSENGINE_API inline const SRenderTransformations& get_transformation() const {return m_transformations;}
		NESSENGINE_API inline void set_position(Point NewPos) {m_transformations.position = NewPos; transformations_update();}
		NESSENGINE_API inline void set_scale(Size NewScale) {m_transformations.scale = NewScale; transformations_update();}
		NESSENGINE_API inline void set_scale(float NewScale) {set_scale(Point(NewScale, NewScale));}
		NESSENGINE_API inline void set_rotation(float NewRotation) {m_transformations.rotation = NewRotation; transformations_update();}
		NESSENGINE_API inline void set_color(Color NewColor) {m_transformations.color = NewColor; transformations_update();}
		NESSENGINE_API inline void set_blend_mode(EBlendModes NewMode) {m_transformations.blend = NewMode; transformations_update();}
		NESSENGINE_API inline void set_zindex(float newZ) {m_transformations.zorder = newZ;}
		NESSENGINE_API inline const Point& get_position() const {return m_transformations.position;}
		NESSENGINE_API inline const Size& get_scale() const {return m_transformations.scale;}
		NESSENGINE_API inline const float get_rotation() const {return m_transformations.rotation;}
		NESSENGINE_API inline const Color get_color() const {return m_transformations.color;}
		NESSENGINE_API inline const EBlendModes get_blend_mode() const {return m_transformations.blend;}
		NESSENGINE_API inline float get_zindex() const {return m_transformations.zorder;}

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update() {}

		// enable/disable rendering of this object
		NESSENGINE_API inline void set_visible(bool Visible) {m_visible = Visible;}
		NESSENGINE_API inline bool is_visible() const {return m_visible;}

		// check if this renderable object is really visible. currently just return if visible, needs to be implemented
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera) = 0;

		// return the absolute transformations of this renderable
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations() = 0;

		// render this object - must be implemented
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera) = 0;

		// change parent
		virtual void __change_parent(RenderableParent* parent) 
		{
			if (m_parent && parent && m_parent != parent) throw IllegalAction("Cannot change object parent without removing from previous parent first!");
			m_parent = parent;
		}
	};

	// renderable pointer
	NESSENGINE_API typedef std::shared_ptr<Renderable> RenderablePtr;
};