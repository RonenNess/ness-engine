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

	ParticlesNode::ParticlesNode(Renderer* renderer, const Size& BounderiesSize) 
		: BaseNode(renderer), m_emit_when_not_in_screen(false), m_bounderies_size(BounderiesSize), m_time_since_last_emit(0.0f) 
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

		// do animation of all the particles
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// try to get current entity as particle
			SpritePtr CurrParticle = ness_ptr_cast<Sprite>(m_entities[i]);

			// if really particle, active its animation
			if (CurrParticle)
			{
//				CurrParticle->do_animation(renderer);
			}
		}
	}

	// check if this particles system is visible using the particles system boundery size (set_bounderies_size())
	bool ParticlesNode::is_really_visible(const CameraPtr& camera)
	{
		// first check if even enabled
		if (!m_visible || m_absolute_trans.color.a <= 0.0f)
			return false;

		// get absolute position
		Ness::Point pos = get_absolute_position();
		if (camera)
		{
			pos -= camera->position;
		}

		if (pos.x - m_bounderies_size.x >= m_renderer->get_target_size().x || pos.y - m_bounderies_size.y >= m_renderer->get_target_size().y 
			|| pos.x + m_bounderies_size.x <= 0 || pos.y + m_bounderies_size.y <= 0 )
		{
			return false;
		}

		return true;
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

		// if not visible and not allowed to emit when not visible, skip
		if (is_really_visible() == false && m_emit_when_not_in_screen == false)
		{
			return;
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