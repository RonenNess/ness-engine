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


#pragma once
#include "renderable_api.h"
#include "node_api.h"

namespace Ness
{
	// remove this entity from parent
	void RenderableAPI::remove_from_parent()
	{
		if (m_parent)
		{
			m_parent->__remove_unsafe(this);
		}
	}

	// change the parent of this object
	void RenderableAPI::__change_parent(NodeAPI* parent) 
	{
		if (m_parent && parent && m_parent != parent) throw IllegalAction("Cannot change object parent without removing from previous parent first!");
		m_parent = parent;
		transformations_update();
	}
};