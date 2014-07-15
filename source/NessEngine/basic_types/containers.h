/**
* Alias to basic container types (vector, list, shared_ptr, ect...)
*
* Author: Ronen Ness
* Since:  26/03/14
*/

#pragma once
#include <vector>
#include <list>
#include <memory>

namespace Ness
{
	template <typename T>
	class Vector : public std::vector<T> {};

	template <typename T>
	class List : public std::list<T> {};

	#define NessSharedPtr std::shared_ptr
	
	#define NESS_MAKE_PTR std::make_shared
};