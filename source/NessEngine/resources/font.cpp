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

#include "font.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	namespace Resources
	{

		LoadedFont::LoadedFont(const String& file_name, unsigned int font_size) : m_font(nullptr), m_file_name(file_name), m_font_size(font_size)
		{
			m_font = TTF_OpenFont( file_name.c_str(), font_size );
			if (!m_font)
			{
				throw FailedToLoadFont(file_name.c_str(), TTF_GetError());
			}
		}

		LoadedFont::~LoadedFont()
		{
			if (m_font)
			{
				TTF_CloseFont(m_font);
			}
		}
	};
};