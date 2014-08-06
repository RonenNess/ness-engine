/**
* Define the Size class.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "point.h"
#include "../exports.h"

namespace Ness
{
	// represent size object (x, y)
	NESSENGINE_API typedef __Point<float> Size;
	NESSENGINE_API typedef __Point<int> Sizei;
};