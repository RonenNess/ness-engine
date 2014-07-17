/**
* Define all the engine exceptions
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <exception>
#include "log.h"
#include "../basic_types/containers.h"

namespace Ness
{

	// raised when a file is not found
	class FileNotFound : public std::exception {
	  private:
		String m_filename;
		String m_full_string;

	  public:
		FileNotFound(const char* fileName) : m_filename(fileName) 
		{
			m_full_string = (String("File not found: ") + m_filename);
			NESS_ERROR(what());
		}
		~FileNotFound() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when failed to load a texture file
	class FailedToLoadTextureFile : public std::exception {
	  private:
		String m_filename;
		String m_reason;
		String m_full_string;

	  public:
		FailedToLoadTextureFile(const char* fileName, const char* reason = "unknown") : m_filename(fileName), m_reason(reason) 
		{
			m_full_string = (String("Failed to load texture file: '") + m_filename + "' reason: " + m_reason);
			NESS_ERROR(what());
		}
		~FailedToLoadTextureFile() throw() {};
		const char *what() const throw() { return m_full_string.c_str(); };
	};

	// raised when failed to load a font file
	class FailedToLoadFont : public std::exception {
	  private:
		String m_filename;
		String m_reason;
		String m_full_string;

	  public:
		FailedToLoadFont(const char* fileName, const char* reason = "unknown") : m_filename(fileName), m_reason(reason) 
		{
			m_full_string = (String("Failed to load font file: '") + m_filename + "' reason: " + m_reason);
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
	
}