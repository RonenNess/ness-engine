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

#ifndef EXPORT_DEF_H_
#define EXPORT_DEF_H_

// WINDOWS EXPORTS (DLL)
#if defined(_WINDOWS) || defined(_WIN32)

// dll exports
#ifdef NESSENGINE_EXPORTS
#define	NESSENGINE_API __declspec(dllexport)

// static linkage (export / import)
#elif defined(NESSENGINE_STATIC)
#define NESSENGINE_API

// default: dll import
#else
#define NESSENGINE_API __declspec(dllimport)
#endif

// LINUX EXPORTS (SO)
#else

#if defined(__GNUC__) && __GNUC__ >= 4
#ifdef NESSENGINE_EXPORTS
#define NESSENGINE_API __attribute__ ((visibility("default")))
#elif defined(NESSENGINE_STATIC)
#define NESSENGINE_API
#else
#define NESSENGINE_API
#endif
#endif
#endif

#endif