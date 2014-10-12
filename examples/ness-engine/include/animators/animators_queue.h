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
* the API for animator object - object that attach to a renderable and "runs" on it every frame.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "animator_api.h"

namespace Ness
{
	// predeclear renderer class
	class Renderer;

	namespace Animators
	{

		/**
		* an AnimatorsQueue is an object capable of holding, activating and managing animators.
		* every object that need to hold and run animators must inherit from this class
		*/
		class AnimatorsQueue
		{
		private:
			Vector<Animators::AnimatorPtr> m_animators;
			Renderer* m_renderer;

		public:
			AnimatorsQueue(Renderer* renderer) : m_renderer(renderer) {}

			// register / remove an animator from this animators queue
			NESSENGINE_API void register_animator(const Animators::AnimatorPtr& animator);
			NESSENGINE_API void remove_animator(const Animators::AnimatorPtr& animator);

			// register / remove an animator with regular pointer
			// WARNING: it's your responsibility to remove animator once deleted or else you will cause seg-fault!
			NESSENGINE_API void register_animator(Animators::AnimatorAPI* animator);
			NESSENGINE_API void remove_animator(Animators::AnimatorAPI* animator);

			
			// run all the animators
			NESSENGINE_API void do_animations();

		};
	};
};