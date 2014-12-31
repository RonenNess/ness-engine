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
* Define the loaded Font class
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include <SDL_ttf.h>
#include "../basic_types/containers.h"
#include "../basic_types/all_basic_types.h"

namespace Ness
{
	namespace Resources
	{
		/**
		* represent a loadable font data
		*/
		class LoadedFont
		{
		private:
			TTF_Font*		m_font;
			String			m_file_name; 
			unsigned int	m_font_size;

		public:
			// create the font from file
			NESSENGINE_API LoadedFont(const String& file_name, unsigned int font_size = 12);

			// destroy the font
			NESSENGINE_API ~LoadedFont();

			// return the font
			inline NESSENGINE_API TTF_Font* font() const {return m_font;}

			// return font properties
			const String& get_file_name() const {return m_file_name;}
  			unsigned int get_font_size() const {return m_font_size;}

		};
	};
};