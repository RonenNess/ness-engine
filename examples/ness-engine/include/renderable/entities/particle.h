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
* A single particle in a particles system
* Author: Ronen Ness
* Since: 10/1014
*/

#pragma once
#include "animated_sprite.h"

namespace Ness
{

	/*
	* represent a single particle in a particles node.
	* note: inherit from AnimatedSprite meaning a particle is basically a sprite, but with the ability to hold and manage animators
	*/
	class Particle : public AnimatedSprite
	{
	protected:
		bool m_move_with_node;
		
	public:

		// create the animated sprite with or without texture
		NESSENGINE_API Particle(Renderer* renderer, ManagedResources::ManagedTexturePtr texture) : AnimatedSprite(renderer, texture), m_move_with_node(false) {}
		NESSENGINE_API Particle(Renderer* renderer, const String& TextureFile) : AnimatedSprite(renderer, TextureFile), m_move_with_node(false) {}
		NESSENGINE_API Particle(Renderer* renderer) : AnimatedSprite(renderer), m_move_with_node(false) {}

		// if true, when moving the particles node owning this particle it will affect the particle as well.
		// if false, the particle will spawn at the position of the node but will not move with it
		// default to false.
		NESSENGINE_API void set_move_with_node(bool Enabled) {m_move_with_node = Enabled;}
		NESSENGINE_API bool get_move_with_node() const {return m_move_with_node;}

		// get absolute transformations of this particle
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();
	};

	// sprite pointer type
	NESSENGINE_API typedef SharedPtr<Particle> ParticlePtr;
};