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

#include "gui_container_api.h"
#include <algorithm>

namespace Ness
{
	namespace Gui
	{

		// empty deleter that does nothing. 
		// this is for the unsafe add/remove functions
		static void EmptyElementDeleter(GuiElementAPI* obj)
		{
		}

		void GuiContainerAPI::__add(const GuiElementPtr& element) 
		{
			m_sons.push_back(element);
		}

		void GuiContainerAPI::__remove(const GuiElementPtr& element) 
		{
			m_sons.erase(std::remove(m_sons.begin(), m_sons.end(), element), m_sons.end());
		}

		void GuiContainerAPI::__add(GuiElementAPI* element)
		{
			__add(GuiElementPtr(element, EmptyElementDeleter));
		}

		void GuiContainerAPI::__remove_unsafe(GuiElementAPI* element)
		{
			__remove(GuiElementPtr(element, EmptyElementDeleter));
		}
	}
}