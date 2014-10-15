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

#include "../../renderer/renderer.h"
#include "tile_map.h"

namespace Ness
{
	const SRenderTransformations& TileMap::get_absolute_transformations()
	{
		// if don't have a parent, return self transformations
		if (!m_parent)
			return m_transformations;

		// calculate this transformations with parent transformations
		m_absolute_transformations = m_transformations;
		m_absolute_transformations.add_transformations(m_parent->get_absolute_transformations());
		return m_absolute_transformations;
	}

	TileMap::TileMap(Renderer* renderer, const String& spriteFile, const Sizei& mapSize, 
		const Size& singleTileSize, const Size& tilesDistance, TCreateTileSprites createSpriteFunction) 
		: NodeAPI(renderer), m_size(mapSize), m_tile_size(singleTileSize), m_extra_tiles_factor(0, 0)
	{
		// set distance between sprites (either sprite size or provided distance)
		m_sprites_distance = (tilesDistance == Ness::Size::ZERO ? singleTileSize : tilesDistance);

		// create the sprites grid
		Pointi index;
		m_sprites.resize(m_size.x);
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			m_sprites[index.x].resize(m_size.y);
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				// create the sprite
				SpritePtr NewSprite;
				if (createSpriteFunction == nullptr)
				{
					NewSprite = ness_make_ptr<Sprite>(this->m_renderer, spriteFile);
				}
				else
				{
					NewSprite = createSpriteFunction(index);
				}
				NewSprite->__change_parent(this);
				
				// arrange current tile in the grid
				arrange_sprite(NewSprite, index);

				// add to matrix of tiles
				m_sprites[index.x][index.y] = NewSprite;
			}
		}
	}

	// arrange a single tile sprite during creation
	void TileMap::arrange_sprite(const SpritePtr& sprite, const Ness::Pointi& index)
	{				
		// set position
		sprite->set_position(Point(
			(float)(index.x * m_sprites_distance.x), 
			(float)(index.y * m_sprites_distance.y)));

		// set size, anchor and default blend mode
		sprite->set_size(m_tile_size);
		sprite->set_anchor(Point(0.5f, 1.0f));
		sprite->set_blend_mode(BLEND_MODE_NONE);

		// set z-index
		sprite->set_zindex(sprite->get_position().y - m_sprites_distance.y);
	}

	void TileMap::set_all_tiles_type(const Pointi& tileIndex, const Sizei& tilesCount)
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				m_sprites[i][j]->set_source_from_sprite_sheet(tileIndex, tilesCount);
			}
		}
	}

	void TileMap::set_all_tiles_source_rect(const Rectangle& sourceRect)
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				m_sprites[i][j]->set_source_rect(sourceRect);
			}
		}
	}

	void TileMap::apply_to_all(TExecuteOnTiles func)
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				func(index, m_sprites[index.x][index.y]);
			}
		}
	}

	void TileMap::transformations_update()
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				m_sprites[index.x][index.y]->transformations_update();
			}
		}
	}

	void TileMap::__get_visible_entities(Containers::Vector<RenderableAPI*>& out_list, const CameraPtr& camera)
	{
		Rectangle range = get_tiles_in_screen(camera);
		for (int i = range.x; i < range.w; i++)
		{
			for (int j = range.y; j < range.h; j++)
			{
				out_list.push_back(m_sprites[i][j].get());
			}
		}
	}
	
	void TileMap::__get_all_entities(Containers::Vector<RenderableAPI*>& out_list, bool breakGroups)
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				out_list.push_back(m_sprites[i][j].get());
			}
		}
	}

	Rectangle TileMap::get_tiles_in_screen(const CameraPtr& camera) 
	{
		Rectangle ret;
		SRenderTransformations trans = get_absolute_transformations(); 

		Point pos = trans.position;
		if (camera)
		{
			pos.x -= camera->position.x;
			pos.y -= camera->position.y;
		}

		ret.x = get_first_tile_in_screen_x(pos);
		ret.y = get_first_tile_in_screen_y(pos);
		ret.w = ret.x + get_tiles_in_screen_x() + 1;
		ret.h = ret.y + get_tiles_in_screen_y() + 2;
		
		put_in_range(ret.x, ret.y);
		put_in_range(ret.w, ret.h);

		return ret;
	}

	void TileMap::put_in_range(int& i, int& j)
	{
		if (i < 0) i = 0;
		if (i > m_size.x) i = m_size.x;
		if (j < 0) j = 0;
		if (j > m_size.y) j = m_size.y;
	}

	int TileMap::get_first_tile_in_screen_x(const Ness::Point& cameraPos)
	{
		return cameraPos.x < 0 ? (int)(((-cameraPos.x - m_tile_size.x) / m_sprites_distance.x)) - m_extra_tiles_factor.x : 0;
	}

	int TileMap::get_first_tile_in_screen_y(const Ness::Point& cameraPos)
	{
		return cameraPos.y < 0 ? (int)(((-cameraPos.y - m_tile_size.y) / m_sprites_distance.y)) - m_extra_tiles_factor.y : 0;
	}

	int TileMap::get_tiles_in_screen_x()
	{
		return (int)((m_renderer->get_target_size().x + m_tile_size.x) / m_sprites_distance.x) + 1 + m_extra_tiles_factor.x * 2;
	}

	int TileMap::get_tiles_in_screen_y()
	{
		return (int)((m_renderer->get_target_size().y + m_tile_size.y) / m_sprites_distance.y) + 1 + m_extra_tiles_factor.y * 2;
	}

	Pointi TileMap::get_index_from_position(const Point& position)
	{
		Pointi index;
		index.x = (int)((position.x + m_tile_size.x * 0.5f) / m_sprites_distance.x);
		index.y = (int)((position.y + m_tile_size.y) / m_sprites_distance.y);
		return index;
	}

	// return position of tile from index
	Point TileMap::get_position_from_index(const Pointi& index) const
	{
		return m_sprites[index.x][index.y]->get_position();
	}

	SpritePtr& TileMap::get_sprite_by_position(const Point& position)
	{
		static SpritePtr empty;
		Pointi index = get_index_from_position(position);
		if (index.x < 0 || index.y < 0 || index.x >= m_size.x || index.y >= m_size.y)
			return empty;
		return get_sprite(index);
	}

	Rectangle TileMap::get_occupied_region() const
	{
		Rectangle ret;
		ret.x = (int)m_absolute_transformations.position.x;
		ret.y = (int)m_absolute_transformations.position.y;
		ret.w = (int)(((m_size.x - 1) * m_sprites_distance.x) + m_tile_size.x);
		ret.h = (int)(((m_size.y - 1) * m_sprites_distance.y) + m_tile_size.y);
		return ret;
	}

	bool TileMap::is_really_visible(const CameraPtr& camera)
	{
		if (!m_visible)
			return false;

		SRenderTransformations trans = get_absolute_transformations(); 
		if (trans.color.a <= 0.0f)
			return false;

		Rectangle TileInScreen = get_tiles_in_screen(camera);
		if (TileInScreen.x >= TileInScreen.w || TileInScreen.y >= TileInScreen.h)
			return false;

		return true;
	}

	void TileMap::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		Rectangle range = get_tiles_in_screen(camera);

		// render all visible tiles
		for (int i = range.x; i < range.w; i++)
		{
			for (int j = range.y; j < range.h; j++)
			{
				m_sprites[i][j]->render(camera);
			}
		}
	}

};