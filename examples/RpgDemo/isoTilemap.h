#include <NessEngine.h>

class IsoTilemap : public Ness::TileMap
{
public:
	// constructor (just call the TileMap's ctor
	IsoTilemap(Ness::Renderer* renderer, const Ness::String& spriteFile, const Ness::Sizei& mapSize, const Ness::Size& singleTileSize = Ness::Size(36, 36), 
			const Ness::Size& tilesDistance = Ness::Size::ZERO, Ness::createTileSprites createSpriteFunction = nullptr) :
	TileMap(renderer, spriteFile, mapSize, singleTileSize, tilesDistance, createSpriteFunction) {}


protected:

	// arrange a single tile sprite during creation - move every second life half size to the left, to make the isometric tiles align
	void arrange_sprite(const Ness::SpritePtr& sprite, const Ness::Pointi& index)
	{
		TileMap::arrange_sprite(sprite, index);
		if (index.y % 2)
		{
			sprite->set_position(sprite->get_position() + Ness::Point(m_sprites_distance.x * 0.5f, 0.0f));
		}
	}
};