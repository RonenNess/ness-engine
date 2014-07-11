#include "resources_manager.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	namespace ManagedResources
	{
		// function to call when a resource shared ptr deletes
		void TextureResourceDeleter(ManagedTexture* texture)
		{
			texture->rc_mng_manager->delete_texture(texture->rc_mng_name);
		}

		void ResourcesManager::delete_texture(const std::string& textureName)
		{	
			// decrease ref count by 1, and if no more refs delete the resource
			m_textures[textureName].ref_count--;
			if (m_textures[textureName].ref_count == 0)
			{
				ManagedTexture* text = m_textures[textureName].texture;
				m_textures.erase(textureName);
				delete text;
			}
		}

		ManagedTexturePtr ResourcesManager::get_texture(const std::string& textureName)
		{
			// if not loaded, load it
			if (m_textures.find(textureName) == m_textures.end())
			{
				TextureInManager& NewEntry = m_textures[textureName];
				NewEntry.texture = new ManagedTexture(m_base_path + textureName, m_renderer, (m_use_color_key ? &m_color_key : nullptr));
				NewEntry.texture->rc_mng_manager = this;
				NewEntry.texture->rc_mng_name = textureName;
				NewEntry.ref_count = 0;
			}

			// return the texture
			m_textures[textureName].ref_count++;
			return ManagedTexturePtr(m_textures[textureName].texture, TextureResourceDeleter);
		}

		ManagedTexturePtr ResourcesManager::create_blank_texture(const std::string& textureName, const Sizei& size)
		{
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
			TextureInManager& NewEntry = m_textures[textureName];
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
		}
	};
};