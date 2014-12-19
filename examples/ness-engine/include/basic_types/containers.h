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
* Alias to basic container types (vector, list, ect...)
*
* Author: Ronen Ness
* Since:  26/03/14
*/

#pragma once
#include "../exports.h"
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace Ness
{
	namespace Containers
	{
		// ness-engine vector
		template <typename T>
		class Vector : public std::vector<T> {};

		// ness-engine list
		template <typename T>
		class List : public std::list<T> {};

		// ness-engine unordered map
		template <typename _KEY, typename _TYPE>
		class UnorderedMap : public std::unordered_map<_KEY, _TYPE> {};
	};
};