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
* Define all the engine exceptions
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <exception>
#include "log.h"
#include "../basic_types/string.h"

namespace Ness
{
	// raised when a file is not found
	class FileNotFound : public std::exception {
	  private:
		String m_file_name;
		String m_full_string;

	  public:
		FileNotFound(const char* file_name) : m_file_name(file_name) 
		{
			m_full_string = (String("File not found: ") + m_file_name);
			NESS_ERROR(what());
		}
		~FileNotFound() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when failed to load a texture file
	class FailedToLoadTextureFile : public std::exception {
	  private:
		String m_file_name;
		String m_reason;
		String m_full_string;

	  public:
		FailedToLoadTextureFile(const char* file_name, const char* reason = "unknown") : m_file_name(file_name), m_reason(reason) 
		{
			m_full_string = (String("Failed to load texture file: '") + m_file_name + "' reason: " + m_reason);
			NESS_ERROR(what());
		}
		~FailedToLoadTextureFile() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when failed to load a font file
	class FailedToLoadFont : public std::exception {
	  private:
		String m_file_name;
		String m_reason;
		String m_full_string;

	  public:
		FailedToLoadFont(const char* file_name, const char* reason = "unknown") : m_file_name(file_name), m_reason(reason) 
		{
			m_full_string = (String("Failed to load font file: '") + m_file_name + "' reason: " + m_reason);
			NESS_ERROR(what());
		}
		~FailedToLoadFont() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when failed to init renderer
	class FailedToInitRenderer : public std::exception {
	  private:
		String m_reason;
		String m_full_string;

	  public:
		FailedToInitRenderer(const char* reason = "unknown") : m_reason(reason) 
		{
			m_full_string =  (String("Failed to init renderer: '") + m_reason);
			NESS_ERROR(what());
		}
		~FailedToInitRenderer() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when failed to init library
	class FailedToInitLib : public std::exception {
	  private:
		String m_lib;
		String m_reason;
		String m_full_string;

	  public:
		FailedToInitLib(const char* lib, const char* reason = "unknown") : m_lib(lib), m_reason(reason) 
		{
			m_full_string = (String("Failed to init lib " + m_lib + ": '") + m_reason);
			NESS_ERROR(what());
		}
		~FailedToInitLib() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when user do something illegal
	class IllegalAction : public std::exception {
	  private:
		String m_reason;
		String m_full_string;

	  public:
		IllegalAction(const char* reason = "unknown") : m_reason(reason) 
		{
			m_full_string = (String("Illegal action: '") + m_reason);
			NESS_ERROR(what());
		}
		~IllegalAction() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when something unexpected happens
	class UnexpectedError : public std::exception {
	  private:
		String m_reason;
		String m_full_string;

	  public:
		UnexpectedError(const char* reason = "unknown") : m_reason(reason) 
		{
			m_full_string = (String("Unexpected error: '") + m_reason);
			NESS_ERROR(what());
		}
		~UnexpectedError() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};
	
}