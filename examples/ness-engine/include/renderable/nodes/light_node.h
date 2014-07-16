/**
* A scene node that renders lighting. 
* basically will render 'ambient_color' all over the screen, and every object rendered will be added as additive rendering and make it brighter.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "basic_node.h"
#include "../entities/canvas.h"

namespace Ness
{
	//class NodeAPI;
	//class Renderer;
	class Light : public Sprite
	{
	public:
		NESSENGINE_API Light(Renderer* renderer, NodeAPI* parent, const std::string& TextureFile, const Color& color) 
			: Sprite(renderer, parent, TextureFile) 
		{
			set_color(color);
			set_anchor(Point::HALF);
		}
	};

	NESSENGINE_API typedef NessSharedPtr<Light> LightPtr;

	class LightNode : public BaseNode
	{
	private:
		CanvasPtr m_canvas;

	public:
		// create the znode
		NESSENGINE_API LightNode(Renderer* renderer, NodeAPI* parent = nullptr);

		// set / get ambient color (ambient == the lighting where there is no light (default is black)
		NESSENGINE_API inline void set_ambient_color(const Color& color) {m_canvas->set_clean_color(color);}
		NESSENGINE_API inline const Color& get_ambient_color() const {return m_canvas->get_clean_color();}

		// create a light
		NESSENGINE_API LightPtr create_light(const std::string& lightTexture, const Color& color = Color::WHITE);

		// render the light node
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

		// add object
		NESSENGINE_API void add(const RenderablePtr& object);
	};

	// scene pointer
	NESSENGINE_API typedef NessSharedPtr<LightNode> LightNodePtr;
};