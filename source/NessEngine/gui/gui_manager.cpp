#include "gui_manager.h"

namespace Ness
{
	namespace Gui
	{
		ContainerPtr GuiManager::create_container(const Pointi& size_in_units)
		{
			ContainerPtr ret = ness_make_ptr<Container>(this, nullptr, size_in_units);
			m_containers.push_back(ret);
			return ret;
		}

		void GuiManager::render()
		{
			for (unsigned int i = 0; i < m_containers.size(); ++i)
			{
				m_containers[i]->render();
			}
		}
	}
}