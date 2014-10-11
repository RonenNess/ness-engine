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

#include "camera.h"
#include "../renderable/renderable_api.h"
#include "../renderer/renderer.h"

namespace Ness
{

	Camera::Camera(Renderer* renderer, const Point& Position) 
		: m_renderer(renderer), position(Position), m_follow_target_speed(0, 0), m_follow_target_offset(0, 0), m_follow_target_max_distance(0, 0), m_bounderies_enabled(false)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::do_animation(Renderer* renderer)
	{
		// focus on target
		if (m_target)
		{
			// get absolute position camera should follow
			Point targetPos = m_target->get_absolute_position() + m_follow_target_offset - renderer->get_target_center();

			// if speed on x axis is 0, i.e. camera follow immediatly, set x position
			if (m_follow_target_speed.x == 0)
			{
				position.x = targetPos.x;
			}
			// else, if camera has speed on x axis:
			else
			{
				// if camera exceedded max distance on x axis, fix it:
				if (m_follow_target_max_distance.x > 0 && abs(position.x - targetPos.x) > m_follow_target_max_distance.x)
				{
					if (position.x < targetPos.x)
					{
						position.x = targetPos.x - m_follow_target_max_distance.x;
					}
					else
					{
						position.x = targetPos.x + m_follow_target_max_distance.x;
					}
				}
				// else move the camera by it's speed on x axis
				else
				{
					float factor = renderer->time_factor() * m_follow_target_speed.x;
					if (position.x < targetPos.x)
					{
						position.x += factor;
						if (position.x > targetPos.x)
							position.x = targetPos.x;
					}
					else if (position.x > targetPos.x)
					{
						position.x -= factor;
						if (position.x < targetPos.x)
							position.x = targetPos.x;
					}
				}
			}

			// if speed on y axis is 0, i.e. camera follow immediatly, set y position
			if (m_follow_target_speed.y == 0)
			{
				position.y = targetPos.y;
			}
			// else, if camera has speed on y axis:
			else
			{
				// if camera exceedded max distance on y axis, fix it:
				if (m_follow_target_max_distance.y > 0 && abs(position.y - targetPos.y) > m_follow_target_max_distance.y)
				{
					if (position.y < targetPos.y)
					{
						position.y = targetPos.y - m_follow_target_max_distance.y;
					}
					else
					{
						position.y = targetPos.y + m_follow_target_max_distance.y;
					}
				}
				// else move the camera by it's speed on y axis
				else
				{
					float factor = renderer->time_factor() * m_follow_target_speed.y;
					if (position.y < targetPos.y)
					{
						position.y += factor;
						if (position.y > targetPos.y)
							position.y = targetPos.y;
					}
					else if (position.y > targetPos.y)
					{
						position.y -= factor;
						if (position.y < targetPos.y)
							position.y = targetPos.y;
					}
				}
			}
		}

		// check bounderies
		if (m_bounderies_enabled)
		{
			if (position.x < m_bounderies.x)
				position.x = (float)m_bounderies.x;
			if (position.y < m_bounderies.y)
				position.y = (float)m_bounderies.y;
			if (position.x + renderer->get_target_size().x > m_bounderies.w)
				position.x = (float)m_bounderies.w - renderer->get_target_size().x;
			if (position.y + renderer->get_target_size().y > m_bounderies.h)
				position.y = (float)m_bounderies.h - renderer->get_target_size().y;
		}
	}

};