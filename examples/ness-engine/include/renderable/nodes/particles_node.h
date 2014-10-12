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
* A scene node for generating animated particles.
* you can use this scene node to generate effects like fire, smoke, sparks, ect..
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"
#include "../entities/particle.h"

namespace Ness
{
	/**
	* a callback function to generate a single particle
	*/
	NESSENGINE_API typedef ParticlePtr (*TParticlesGenerator)();

	/** 
	* emitting settings of a particles node.
	* example of usage:
	* SParticlesNodeSettings data;
	* data.emitting_interval = 1.0f;
	* data.chance_to_emit = 25;
	* data.min_particles_emit = 5;
	* data.max_particles_emit = 10;
	* data.max_particles_count = 100;
	*
	* the settings above will invoke emitting event every second (emitting_interval=1.0f). in every emitting event it will first roll a random 
	* a number between 0 to 100, and only if value is below 25 (chance_to_emit) it will emit particles. every time the node will actually amit, it
	* will generate 5 to 10 new particles (min_particles_emit - max_particles_emit), as long as the total count of particles does not exceed 100 (max_particles_count).
	*/
	struct SParticlesNodeEmitSettings
	{
		float				emitting_interval;		// how often, in seconds, should the node emit particles. for example, 1.5 will emit particles every 1.5 seconds
		unsigned char		chance_to_emit;			// whenever emitting interval expires and its time to emit, this is the chance (in percents) to emit something
		unsigned int		min_particles_emit;		// when its time to emit particles AND chance to emit rolled ok, how many particles it will have to emit
		unsigned int		max_particles_emit;		// when its time to emit particles AND chance to emit rolled ok, how many particles it will emit at most
		unsigned int		max_particles_count;	// how many particles there can be in this particles node. will not emit new particles if blocked by quota
		TParticlesGenerator	particles_generator;	// the callback to generate the particles

		SParticlesNodeEmitSettings(float EmitInterval = 1.0f, unsigned char ChanceToEmit = 100, unsigned int MinEmit = 1, unsigned int MaxEmit = 3, unsigned int MaxCount = 100) :
			emitting_interval(EmitInterval), chance_to_emit(ChanceToEmit), min_particles_emit(MinEmit), max_particles_emit(MaxEmit), max_particles_count(MaxCount), particles_generator(nullptr)
		{ }
	};

	/**
	* A node for generating animated particles
	*/
	class ParticlesNode : public BaseNode, public Animators::AnimatorAPI
	{
	private:
		SParticlesNodeEmitSettings		m_settings;
		bool							m_emit_when_not_in_screen;
		Size							m_bounderies_size;
		float							m_time_since_last_emit;

	public:
		// create the particles node and register/unregister to animators queue automatically
		// BounderiesSize is the estimated size of the whole particles system. determine visibilty and 
		// will not emit particles when out of screen, unless 'set_emit_when_not_in_screen()' is set.
		NESSENGINE_API ParticlesNode(Renderer* renderer, const Size& BounderiesSize);
		NESSENGINE_API ~ParticlesNode();

		// set if should emit when out of screen
		// if true, it means this particles node will continue emitting particles even when out of screen.
		// if false, it will automatically pause when out of screen bounderies.
		// note: you can use set_bounderies_size to determine the total size of this particles system
		NESSENGINE_API inline void set_emit_when_not_in_screen(bool Enabled) {m_emit_when_not_in_screen = Enabled;}

		// estimated size determines if this particles node is in-screen or not
		NESSENGINE_API inline void set_bounderies_size(const Size& size) {m_bounderies_size = size * 0.5f;}

		// check if this particles system is visible using the particles system boundery size (set_bounderies_size())
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set the emit settings of this node
		NESSENGINE_API inline void set_emit_settings(const SParticlesNodeEmitSettings& settings) {m_settings = settings;}

		// animate the particles in this node & emit new particles when needed
		NESSENGINE_API virtual void do_animation(Renderer* renderer);

		// invoke emit event. will generate particles based on emit settings (might not generate anything if 
		// chance to emit is less then 100)
		NESSENGINE_API void invoke_emit();
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<ParticlesNode> ParticlesNodePtr;
};