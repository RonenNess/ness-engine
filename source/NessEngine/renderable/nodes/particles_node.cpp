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
		: BaseNode(renderer), m_emit_while_not_visible(false), m_bounderies_size(BounderiesSize), m_time_since_last_emit(0.0f), m_is_currently_visible(true)
	{
		renderer->__register_animator_unsafe(this);
	}

	ParticlesNode::~ParticlesNode()
	{
		m_renderer->__remove_animator_unsafe(this);
	}

	void ParticlesNode::remove_from_parent()
	{
		if (m_parent)
		{
			m_parent->__remove_unsafe(this);
		}
		remove_from_animation_queue();
	}
	
	void ParticlesNode::do_animation(Renderer* renderer)
	{
		// do animation of all the particles
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// try to get current entity as particle
			ParticlePtr CurrParticle = ness_ptr_cast<Particle>(m_entities[i]);

			// if really particle, active its animation
			if (CurrParticle)
			{
				CurrParticle->do_animation(renderer);
			}
		}

		// check if should stop
		if (((m_settings.stop_after_count > 0) && (m_total_particles_generated >= m_settings.stop_after_count)) || 
			((m_settings.stop_after_seconds > 0.0f) && (m_time_actived >= m_settings.stop_after_seconds)))
		{
			if (m_settings.remove_when_done && get_particles_count() == 0)
			{
				remove_from_parent();
			}
			return;
		}

		// increase time passed since last time we emitted
		m_time_since_last_emit += renderer->time_factor();
		m_time_actived += renderer->time_factor();

		// check if its time to emit
		if (m_time_since_last_emit >= m_settings.emitting_interval)
		{
			invoke_emit();
		}
	}

	unsigned int ParticlesNode::get_particles_count() const
	{
		unsigned int ret = 0;
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// try to get current entity as particle
			if (ness_ptr_cast<Particle>(m_entities[i]))
			{
				ret++;
			}
		}
		return ret;
	}

	bool remove_particles(const RenderablePtr& entity)
	{
		return (ness_ptr_cast<Particle>(entity));
	}

	void ParticlesNode::reset(bool RemoveExistingParticles)
	{
		m_time_actived = 0.0f;
		m_total_particles_generated = 0;
		if (RemoveExistingParticles)
		{
			m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), remove_particles), m_entities.end());
		}
	}

	void ParticlesNode::set_emit_settings(const SParticlesNodeEmitSettings& settings, bool ResetCounters) 
	{
		m_settings = settings;
		if (ResetCounters) 
		{
			reset(false);
		}
	}

	// check if this particles system is visible using the particles system boundery size (set_bounderies_size())
	bool ParticlesNode::is_really_visible(const CameraPtr& camera)
	{
		// first check if even enabled
		if (!m_visible || m_absolute_trans.color.a <= 0.0f)
		{
			return false;
		}

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

	void ParticlesNode::render(const CameraPtr& camera)
	{
		BaseNode::render(camera);
		if (m_emit_while_not_visible == false)
		{
			m_is_currently_visible = is_really_visible(camera);
		}
	}

	void ParticlesNode::invoke_emit()
	{
		// zero the time since last emit
		m_time_since_last_emit = 0.0f;

		// make sure we got emitting function
		if (m_settings.particles_emitter == nullptr)
		{
			return;
		}

		// increase emitting interval
		m_settings.emitting_interval += m_settings.emit_fading_rate;

		// check if we should skip this emittion event based on chance_to_emit
		if (m_settings.chance_to_emit < 100)
		{
			if (rand() % 100 > m_settings.chance_to_emit)
			{
				return;
			}
		}

		// if not visible and not allowed to emit when not visible, skip
		if ((m_is_currently_visible == false) && (m_emit_while_not_visible == false))
		{
			return;
		}

		// check if we don't have too much already
		int QuotaLimit = (int)m_settings.max_particles_count - (int)get_particles_count();
		if (QuotaLimit <= 0)
			return;

		// calculate how many particles to emit
		unsigned int particlesToEmit;
		if (m_settings.max_particles_emit > m_settings.min_particles_emit)
		{
			 particlesToEmit = m_settings.min_particles_emit + (rand() % (m_settings.max_particles_emit - m_settings.min_particles_emit));
		}
		else
		{
			particlesToEmit = m_settings.min_particles_emit;
		}
		if (particlesToEmit > (unsigned int)QuotaLimit)
			particlesToEmit = QuotaLimit;

		// check total particles limit (if exists)
		if (m_settings.stop_after_count > 0)
		{
			if (m_total_particles_generated >= m_settings.stop_after_count)
			{
				return;
			}

			unsigned int particlesLeft = m_settings.stop_after_count - m_total_particles_generated;
			if (particlesToEmit > particlesLeft)
			{
				particlesToEmit = particlesLeft;
			}
		}

		// generate the new particles!
		for (unsigned int i = 0; i < particlesToEmit; i++)
		{
			ParticlePtr NewParticle = m_settings.particles_emitter->emit_particle(m_renderer);
			if (NewParticle)
			{
				if (NewParticle->get_move_with_node() == false)
				{
					NewParticle->add_position(get_absolute_position());
				}
				add(NewParticle);
				m_total_particles_generated += 1;
			}
		}
	}

			
};