/**
* Canvas is a special type of sprite - instead of using existing texture it creates an empty texture and allow other nodes to actually render on itself.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "sprite.h"

namespace Ness
{
	class Canvas : public Sprite
	{
	private:
		bool				m_auto_clear;
		unsigned int		m_last_clear_time;
		Color				m_clean_color;
	public:
		// create the canvas.
		// TextureName is the name of the texture in the resource manager
		// size is the size of the canvas texture, if ZERO will use entire screen size
		NESSENGINE_API Canvas(Renderer* renderer, const std::string& NewTextureName, const Sizei& size = Sizei::ZERO);

		// if true, will clear this canvas after every frame rendered
		NESSENGINE_API inline void set_auto_clean(bool enabled) {m_auto_clear = enabled;}

		// clear the canvas
		NESSENGINE_API void clear();

		// set clean color
		NESSENGINE_API inline void set_clean_color(const Color& color) {m_clean_color = color;}
		NESSENGINE_API inline const Color& get_clean_color() {return m_clean_color;}

		// render the canvas
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	typedef NessSharedPtr<Canvas> CanvasPtr;

};
