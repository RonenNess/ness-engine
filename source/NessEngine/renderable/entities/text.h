/**
* A renderable text
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "entity.h"
#include "../../managed_resources/managed_texture.h"
#include "../../primitives/rectangle.h"

namespace Ness
{

	// the renderable text class
	class Text : public Entity
	{
	protected:
		ManagedResources::ManagedTexturePtr		m_texture;
		Rectangle								m_source_rect;
		
	public:

		// create the sprite with texture
		NESSENGINE_API Text(Renderer* renderer, NodeAPI* parent, ManagedResources::ManagedTexturePtr font, const std::string& text);
		NESSENGINE_API Text(Renderer* renderer, NodeAPI* parent, const std::string& FontFile, const std::string& text);

		// change texture
		NESSENGINE_API inline void change_font(ManagedResources::ManagedTexturePtr NewTexture) {m_texture = NewTexture;}

		// return the texture this sprite uses
		NESSENGINE_API inline const ManagedResources::ManagedTexturePtr& get_font() const {return m_texture;}
		NESSENGINE_API inline ManagedResources::ManagedTexturePtr get_font() {return m_texture;}

	protected:

		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// sprite pointer type
	NESSENGINE_API typedef std::shared_ptr<Text> TextPtr;
};