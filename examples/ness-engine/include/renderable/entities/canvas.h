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
	public:
		// create the canvas.
		// TextureName is the name of the texture in the resource manager
		// size is the size of the canvas texture, if ZERO will use entire screen size
		NESSENGINE_API Canvas(Renderer* renderer, NodeAPI* parent, const std::string& NewTextureName, const Sizei& size = Sizei::ZERO);

		// if true, will clear this canvas after every frame rendered
		NESSENGINE_API inline void set_auto_clean(bool enabled) {m_auto_clear = enabled;}

		// clear the canvas
		NESSENGINE_API void clear();

		// render the canvas
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	typedef NessSharedPtr<Canvas> CanvasPtr;

};
