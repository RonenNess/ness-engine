/**
* Define all the engine exceptions
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <exception>
#include <string>

namespace Ness
{

	// raised when a file is not found
	class FileNotFound : public std::exception {
	  private:
		std::string m_filename;

	  public:
		FileNotFound(const char* fileName) : m_filename(fileName) {};
		~FileNotFound() throw() {};
		const char *what() const throw() { return (std::string("File not found: ") + this->m_filename).c_str(); };
	};

	// raised when failed to load a texture file
	class FailedToLoadTextureFile : public std::exception {
	  private:
		std::string m_filename;
		std::string m_reason;

	  public:
		FailedToLoadTextureFile(const char* fileName, const char* reason = "unknown") : m_filename(fileName), m_reason(reason) {};
		~FailedToLoadTextureFile() throw() {};
		const char *what() const throw() { return (std::string("Failed to load texture file: '") + this->m_filename + "' reason: " + m_reason).c_str(); };
	};

	// raised when failed to load a font file
	class FailedToLoadFont : public std::exception {
	  private:
		std::string m_filename;
		std::string m_reason;

	  public:
		FailedToLoadFont(const char* fileName, const char* reason = "unknown") : m_filename(fileName), m_reason(reason) {};
		~FailedToLoadFont() throw() {};
		const char *what() const throw() { return (std::string("Failed to load font file: '") + this->m_filename + "' reason: " + m_reason).c_str(); };
	};

	// raised when failed to init renderer
	class FailedToInitRenderer : public std::exception {
	  private:
		std::string m_reason;

	  public:
		FailedToInitRenderer(const char* reason = "unknown") : m_reason(reason) {};
		~FailedToInitRenderer() throw() {};
		const char *what() const throw() { return (std::string("Failed to init renderer: '") + m_reason).c_str(); };
	};

	// raised when failed to init library
	class FailedToInitLib : public std::exception {
	  private:
		std::string m_lib;
		std::string m_reason;

	  public:
		FailedToInitLib(const char* lib, const char* reason = "unknown") : m_lib(lib), m_reason(reason) {};
		~FailedToInitLib() throw() {};
		const char *what() const throw() { return (std::string("Failed to init lib " + m_lib + ": '") + m_reason).c_str(); };
	};

	// raised when user do something illegal
	class IllegalAction : public std::exception {
	  private:
		std::string m_reason;

	  public:
		IllegalAction(const char* reason = "unknown") : m_reason(reason) {};
		~IllegalAction() throw() {};
		const char *what() const throw() { return (std::string("Illegal action: '") + m_reason).c_str(); };
	};
	
}