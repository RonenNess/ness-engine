#include <NessEngine.h>

class IsoTilemap : public Ness::TileMap
{
public:
	// constructor (just call the TileMap's ctor
	IsoTilemap(Ness::Renderer* renderer, const Ness::String& spriteFile, const Ness::Sizei& mapSize, const Ness::Size& singleTileSize = Ness::Size(36, 36), 
			const Ness::Size& tilesDistance = Ness::Size::ZERO, Ness::createTileSprites createSpriteFunction = nullptr) :
	TileMap(renderer, spriteFile, mapSize, singleTileSize, tilesDistance, createSpriteFunction) 
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				m_sprites_distance.y = m_tile_size.y * 0.45f;
				if (j % 2) m_sprites[i][j]->set_position(m_sprites[i][j]->get_position() - Ness::Point(m_tile_size.x * 0.5f, 0.0f));
				m_sprites[i][j]->set_position(Ness::Point(m_sprites[i][j]->get_position().x, j * m_sprites_distance.y));
				m_sprites[i][j]->set_blend_mode(Ness::BLEND_MODE_BLEND);
				
			}
		}
	}
};