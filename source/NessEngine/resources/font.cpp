#include "font.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	namespace Resources
	{

		LoadedFont::LoadedFont(const std::string& fileName, int fontSize) : m_font(nullptr)
		{
			m_font = TTF_OpenFont( fileName.c_str(), fontSize );
			if (!m_font)
			{
				throw FailedToLoadFont(fileName.c_str(), TTF_GetError());
			}
		}

		LoadedFont::~LoadedFont()
		{
			if (m_font)
			{
				TTF_CloseFont(m_font);
			}
		}
	};
};