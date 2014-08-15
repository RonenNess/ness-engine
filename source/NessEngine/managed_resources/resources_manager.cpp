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

#include "resources_manager.h"
#include "../exceptions/exceptions.h"
#include "../exceptions/log.h"
#include "../renderer/renderer.h"

namespace Ness
{
	namespace ManagedResources
	{
		// function to call when a texture shared ptr deletes
		void TextureResourceDeleter(ManagedTexture* texture)
		{
			texture->rc_mng_manager->__delete_texture(texture->rc_mng_name);
		}

		// function to call when a font shared ptr deletes
		void FontResourceDeleter(ManagedFont* font)
		{
			font->rc_mng_manager->__delete_font(font->rc_mng_name);
		}

		void ResourcesManager::__delete_texture(const String& textureName)
		{	
			// decrease ref count by 1, and if no more refs delete the resource
			m_textures[textureName].ref_count--;
			if (m_textures[textureName].ref_count == 0)
			{
				NESS_LOG(("rc_manager: delete no longer used texture: " + textureName).c_str());
				ManagedTexture* text = m_textures[textureName].texture;
				m_textures.erase(textureName);
				delete text;
			}
		}

		void ResourcesManager::__delete_font(const String& fontName)
		{	
			// decrease ref count by 1, and if no more refs delete the resource
			m_fonts[fontName].ref_count--;
			if (m_fonts[fontName].ref_count == 0)
			{
				NESS_LOG(("rc_manager: delete no longer used font: " + fontName).c_str());
				ManagedFont* font = m_fonts[fontName].font;
				m_fonts.erase(fontName);
				delete font;
			}
		}

		ManagedTexturePtr ResourcesManager::get_texture(const String& textureName)
		{
			// if not loaded, load it
			if (m_textures.find(textureName) == m_textures.end())
			{
				NESS_LOG(("rc_manager: load texture: " + textureName).c_str());
				__STextureInManager& NewEntry = m_textures[textureName];
				NewEntry.texture = new ManagedTexture(m_base_path + textureName, m_renderer->__sdl_renderer(), (m_use_color_key ? &m_color_key : nullptr));
				NewEntry.texture->rc_mng_manager = this;
				NewEntry.texture->rc_mng_name = textureName;
				NewEntry.ref_count = 0;
			}

			// return the texture
			m_textures[textureName].ref_count++;
			return ManagedTexturePtr(m_textures[textureName].texture, TextureResourceDeleter);
		}

		NESSENGINE_API ManagedFontPtr ResourcesManager::get_font(const String& fontName, int fontSize)
		{
			// get name in hash
			String fullName = fontName + ness_to_string((long long)fontSize);

			// if not loaded, load it
			if (m_fonts.find(fullName) == m_fonts.end())
			{
				NESS_LOG(("rc_manager: load font: " + fullName).c_str());
				__SFontInManager& NewEntry = m_fonts[fullName];
				NewEntry.font = new ManagedFont(m_base_path + fontName, fontSize);
				NewEntry.font->rc_mng_manager = this;
				NewEntry.font->rc_mng_name = fullName;
				NewEntry.ref_count = 0;
			}

			// return the texture
			m_fonts[fullName].ref_count++;
			return ManagedFontPtr(m_fonts[fullName].font, FontResourceDeleter);
		}

		ManagedTexturePtr ResourcesManager::create_blank_texture(const String& textureName, const Sizei& size)
		{
			NESS_LOG(("rc_manager: create new empty texture: " + textureName).c_str());

			// if texture with that name exist, assert
			if (m_textures.find(textureName) != m_textures.end())
			{
				throw IllegalAction(("Texture with the name of '" + textureName + "' already exist!").c_str());
			}

			// convert size if zero
			Sizei TexSize = (size == Sizei::ZERO ? m_renderer->get_screen_size() : size);

			// create the texture
			__STextureInManager& NewEntry = m_textures[textureName];
			NewEntry.texture = new ManagedTexture(m_renderer->__sdl_renderer(), TexSize);
			NewEntry.texture->rc_mng_manager = this;
			NewEntry.texture->rc_mng_name = textureName;
			NewEntry.ref_count = 0;

			// return it
			m_textures[textureName].ref_count++;
			return ManagedTexturePtr(m_textures[textureName].texture, TextureResourceDeleter);
		}

		ResourcesManager::ResourcesManager() : m_use_color_key(false), m_renderer(nullptr)
		{
		}

		ResourcesManager::~ResourcesManager()
		{
			m_textures.clear();
			m_fonts.clear();
		}
	};
};