#include "../renderer/renderer.h"
#include "tile_map.h"
#include "node.h"

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

	TileMap::TileMap(Renderer* renderer, RenderableParent* parent, const std::string& spriteFile, Sizei mapSize, Size singleTileSize) : Renderable(renderer, parent),
		m_size(mapSize), m_tile_size(singleTileSize)
	{
		m_sprites.resize(m_size.x);
		for (int i = 0; i < m_size.x; i++)
		{
			m_sprites[i].resize(m_size.y);
			for (int j = 0; j < m_size.y; j++)
			{
				// create the sprite
				SpritePtr NewSprite(new Sprite(this->m_renderer, this, spriteFile));
				
				// set position
				NewSprite->set_position(Point(
					(float)(i * m_tile_size.x) + (m_tile_size.x * 0.5f), 
					(float)(j * m_tile_size.y) + m_tile_size.y));

				// set size, anchor and default blend mode
				NewSprite->set_size(m_tile_size);
				NewSprite->set_anchor(Point(0.5f, 1.0f));
				NewSprite->set_blend_mode(BLEND_MODE_NONE);

				// set z-index
				NewSprite->set_zindex((int)(j * m_tile_size.y));

				// add to matrix of tiles
				m_sprites[i][j] = NewSprite;
			}
		}
	}

	void TileMap::set_all_tiles_type(const Pointi& step, const Sizei stepsCount)
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				m_sprites[i][j]->set_source_from_sprite_sheet(step, stepsCount);
			}
		}
	}

	void TileMap::apply_to_all(executeOnTiles func)
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

	// get range of tiles that are within the map
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

		// get first and last tiles to render
		int startI = pos.x < 0 ? (int)(-pos.x / m_tile_size.x) : 0;
		if (startI >= m_size.x) startI = m_size.x - 1;
		int startJ = pos.y < 0 ? (int)(-pos.y / m_tile_size.y) : 0;
		if (startJ >= m_size.y) startJ = m_size.y - 1;
		int endI = startI + (int)(m_renderer->get_target_size().x / m_tile_size.x) + 2;
		int endJ = startJ + (int)(m_renderer->get_target_size().y / m_tile_size.y) + 2;
		if (endI >= m_size.x) endI = m_size.x - 1;
		if (endJ >= m_size.y) endJ = m_size.y - 1;
		ret.x = startI;
		ret.y = startJ;
		ret.w = endI;
		ret.h = endJ;
		return ret;
	}

	SpritePtr& TileMap::get_sprite_by_position(const Point& position)
	{
		static SpritePtr empty;
		Pointi index;
		index.x = (int)(position.x / m_tile_size.x);
		index.y = (int)(position.y / m_tile_size.y);
		if (index.x < 0 || index.y < 0 || index.x >= m_size.x || index.y >= m_size.y)
			return empty;
		return get_sprite(index);
	}

	bool TileMap::is_really_visible(const CameraPtr& camera)
	{
		if (!m_visible)
			return false;

		SRenderTransformations trans = get_absolute_transformations(); 
		if (trans.color.a <= 0.0f)
			return false;

		Point pos = trans.position;
		pos.x -= camera->position.x;
		pos.y -= camera->position.y;

		int startI = pos.x < 0 ? (int)(-pos.x / m_tile_size.x) : 0;
		if (startI >= m_size.x)
			return false;
		
		int startJ = pos.y < 0 ? (int)(-pos.y / m_tile_size.y) : 0;
		if (startJ >= m_size.y)
			return false;
		
		int endI = startI + (int)(m_renderer->get_target_size().x / m_tile_size.x) + 2;
		if (endI <= 0)
			return false;
		
		int endJ = startJ + (int)(m_renderer->get_target_size().y / m_tile_size.y) + 2;
		if (endJ <= 0)
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