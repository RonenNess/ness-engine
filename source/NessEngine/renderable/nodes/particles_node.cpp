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

#include "particles_node.h"
#include <algorithm>
#include "../../renderer/renderer.h"
#include "../entities/all_entities.h"

namespace Ness
{

	ParticlesNode::ParticlesNode(Renderer* renderer) : BaseNode(renderer), m_time_since_last_emit(0.0f) 
	{
		renderer->register_animator(this);
	}

	ParticlesNode::~ParticlesNode()
	{
		m_renderer->remove_animator(this);
	}
	
	void ParticlesNode::do_animation(Renderer* renderer)
	{
		// increase time passed since last time we emitted
		m_time_since_last_emit += renderer->time_factor();

		// check if its time to emit
		if (m_time_since_last_emit >= m_settings.emitting_interval)
		{
			invoke_emit();
		}
	}

	void ParticlesNode::invoke_emit()
	{
		// zero the time since last emit
		m_time_since_last_emit = 0.0f;

		// make sure we got emitting function
		if (m_settings.particles_generator == nullptr)
		{
			return;
		}

		// check if we should skip this emittion event based on chance_to_emit
		if (m_settings.chance_to_emit < 100)
		{
			if (rand() % 100 > m_settings.chance_to_emit)
			{
				return;
			}
		}

		// check if we don't have too much already
		int QuotaLimit = m_settings.max_particles_count - m_entities.size();
		if (QuotaLimit <= 0)
			return;

		// calculate how many particles to emit
		unsigned int particlesToEmit = m_settings.min_particles_emit + (rand() % (m_settings.max_particles_emit - m_settings.min_particles_emit));
		if (particlesToEmit > (unsigned int)QuotaLimit)
			particlesToEmit = QuotaLimit;

		// generate the new particles!
		for (unsigned int i = 0; i < particlesToEmit; i++)
		{
			add(m_settings.particles_generator());
		}

	}

			
};