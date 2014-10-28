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

#include "particle.h"
#include "../nodes/node.h"

namespace Ness
{
	const SRenderTransformations& Particle::get_absolute_transformations()
	{
		// if don't have a parent, return self transformations
		if (!m_parent)
		{
			// recalculate the target rectangle (uses m_absolute_transformations)
			if (m_need_transformations_update)
			{
				m_absolute_transformations = m_transformations;
				calc_target_rect();
				m_need_transformations_update = false;
			}
			return m_transformations;
		}

		// don't need update?
		if (!m_need_transformations_update)
		{
			return m_absolute_transformations;
		}

		// calculate this transformations with parent transformations
		m_absolute_transformations = m_transformations;
		m_absolute_transformations.add_transformations(m_parent->get_absolute_transformations());
		
		// if not moving with node need to disable parent position
		if (m_move_with_node == false)
		{
			m_absolute_transformations.position = get_position() * m_parent->get_absolute_transformations().scale;
		}

		// no longer need transformation update
		m_need_transformations_update = false;

		// update target rect
		calc_target_rect();

		// return transformations
		return m_absolute_transformations;
	}
};