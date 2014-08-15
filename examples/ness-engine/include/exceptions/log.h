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
* ness-engine Log macros, print to the console window only when in debug mode.
* Author: Ronen Ness
*/

#pragma once

#ifdef _DEBUG

	#define NESS_LOG(__str_)				printf("NessLog: %s\r\n", __str_)
	#define NESS_ERROR(__str_)				printf("NessErr: %s\r\n", __str_)

#else

	#define NESS_LOG(__str_) 
	#define NESS_ERROR(__str_) 

#endif