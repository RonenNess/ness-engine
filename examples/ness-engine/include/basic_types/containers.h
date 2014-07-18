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
#include <string>

namespace Ness
{
	template <typename T>
	class Vector : public std::vector<T> {};

	template <typename T>
	class List : public std::list<T> {};

	#define NessSharedPtr std::shared_ptr
	
	#define ness_make_ptr std::make_shared

	#define ness_ptr_cast std::dynamic_pointer_cast

	typedef std::string String;
	#define ness_to_string std::to_string
};