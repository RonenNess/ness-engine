/**
* Alias to basic container types (vector, list, shared_ptr, ect...)
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
	// ness-engine vector
	template <typename T>
	class Vector : public std::vector<T> {};

	// ness-engine list
	template <typename T>
	class List : public std::list<T> {};

	// ness-engine unordered map
	template <typename _KEY, typename _TYPE>
	class UnorderedMap : public std::unordered_map<_KEY, _TYPE> {};

	// ness-engine shared ptr
	#define SharedPtr std::shared_ptr

	// create a shared ptr.
	// usage: SpritePtr NewSprite = ness_make_ptr<Sprite>(renderer, "sometexture.png");
	#define ness_make_ptr std::make_shared

	// cast a ness-pointer.
	// usage: Ness::SpritePtr sprite = ness_ptr_cast<Ness::Sprite>( SomeRenderablePtr );
	// will return empty ptr if type cannot be cast to sprite.
	#define ness_ptr_cast std::dynamic_pointer_cast

	// ness-engine strings
	typedef std::string String;
	#define ness_to_string std::to_string
	#define ness_int_to_string(__int) std::to_string((long long)__int)
	#define ness_float_to_string(__int) std::to_string((long double)__int)
};