#include "resources_manager.h"
#include "../exceptions/exceptions.h"
#include "../exceptions/log.h"

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
				NewEntry.texture = new ManagedTexture(m_base_path + textureName, m_renderer, (m_use_color_key ? &m_color_key : nullptr));
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
			Sizei TexSize = size;
			if (TexSize == Sizei::ZERO)
			{
				SDL_GetRendererOutputSize(m_renderer, &TexSize.x, &TexSize.y);
			}

			// create the texture
			__STextureInManager& NewEntry = m_textures[textureName];
			NewEntry.texture = new ManagedTexture(m_renderer, TexSize);
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