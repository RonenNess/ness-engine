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

#include "scene.h"
#include "camera.h"
#include "../renderer/renderer.h"

namespace Ness
{
	void Scene::render_on_viewport(const ViewportPtr& viewport, const CameraPtr& camera)
	{
		// create a temporary camera to set viewport source position
		CameraPtr temp_cam = m_renderer->create_camera();
		if (camera)
		{
			*temp_cam = *camera; 
		}
		temp_cam->position += viewport->get_source_position();

		// render the scene on the 
		m_renderer->push_render_target(viewport->get_canvas_texture());
		render(temp_cam);
		m_renderer->pop_render_target();

	}
};