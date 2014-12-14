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

/**
* A renderable tilemap, consist of a matrix of sprites.
* have special optimizations for culling and efficient rendering
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../entities/sprite.h"
#include "../transformable_api.h"
#include "../node_api.h"
#include "../../basic_types/containers.h"

namespace Ness
{
	// callback function to run on all tiles
	NESSENGINE_API typedef void (*TExecuteOnTiles)(const Ness::Pointi& index, const Ness::SpritePtr& tile);

	// callback function to create custom sprite types for the tilemap
	NESSENGINE_API typedef SpritePtr (*TCreateTileSprites)(const Pointi& index);

	/* 
	* TileMap is a special node that creates a grid of sprites, mostly useable to represent the ground in an rpg game or the
	* platforms in a platformer. highly optimized!
	*/
	class TileMap : public NodeAPI
	{
	protected:
		Sizei													m_size;						// size of the tilemap
		Size													m_sprites_distance;			// distance between sprites
		Size													m_tile_size;				// size of a single tile
		Containers::Vector< Containers::Vector<SpritePtr> >		m_sprites;					// the sprites matrix
		SRenderTransformations									m_absolute_transformations;	// absolute transformations of this tilemap
		Sizei													m_extra_tiles_factor;		// extra tiles to render (count in screen) on eatch side of x and y axis
		Point													m_tiles_anchor;				// the tiles default anchor

	public:

		// create the tilemap
		// spriteFile - default texture file to use for all tiles. you can change texture for specific tiles using get_sprite()
		// mapSize - how many tiles there are on rows and columns
		// singleTileSize - the size in pixels of a single tile
		// tilesDistance - the distance between tiles. if zero, will use the tile size
		// createSpriteFunction - if provided, a function that creates the sprite objects for this tilemap. 
		//		note: use it only if you want to use different objects for this tilemap (must inherit from Sprite). 
		//		don't use this for custom properties per-sprite, since they will be overrided anyway when arranged into the tilemap grid.
		NESSENGINE_API TileMap(Renderer* renderer, const String& spriteFile, const Sizei& mapSize, const Size& singleTileSize = Size(36, 36), 
			const Size& tilesDistance = Size::ZERO, TCreateTileSprites createSpriteFunction = nullptr);

		// return the absolute transformations of this tilemap
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// set extra tiles to render in screen for x and y axis.
		// what is it for? if your normal tiles size is 32x32 but sometimes you have tiles of 32x64, you can set it to (0, 1), meaning you
		// will count additional tile from top and bottom when rendering the tiles in screen
		NESSENGINE_API inline void set_extra_tiles_in_screen(const Ness::Sizei& extra) {m_extra_tiles_factor = extra;}

		// set the anchor of all the tiles.
		// its important to use this function and not do it manually, because this function also effect the 
		// function to pick tile from position.
		NESSENGINE_API void set_tiles_anchor(const Point& anchor);

		// return tilemap params
		NESSENGINE_API inline const Sizei& get_map_size() const {return m_size;}
		NESSENGINE_API inline const Size& get_sprites_distance() const {return m_sprites_distance;}
		NESSENGINE_API inline const Size& get_tiles_size() const {return m_tile_size;}

		// return the total region that this nodes map take (based on current absolute position, nodes matrix size, and distance between nodes)
		// note: this is estimation based on the starting configuration you provided the constructor. this will not work if you made a custom
		// matrix of tiles or tempered with the tiles map.
		NESSENGINE_API Rectangle get_occupied_region() const;

		// direct access to son entities (note: son entities are in vector so efficiecny is alright here)
		NESSENGINE_API virtual unsigned int get_sons_count() const {return m_size.x * m_size.y;}
		NESSENGINE_API virtual RenderablePtr get_son(unsigned int index) {return m_sprites[index % m_size.x][index / m_size.x];}

		// return if need transformations udpate (always false for tilemap)
		NESSENGINE_API virtual bool need_transformations_update() {return false;}

		// get a specific sprite by index
		NESSENGINE_API inline SpritePtr& get_sprite(const Pointi& index) {return m_sprites[index.x][index.y];}

		// get a specific sprite by position
		// return empty if out of range
		NESSENGINE_API virtual SpritePtr& get_sprite_by_position(const Point& position);

		// return index of tile from position
		NESSENGINE_API virtual Pointi get_index_from_position(const Point& position);

		// return position of tile from index
		NESSENGINE_API virtual Point get_position_from_index(const Pointi& index) const;

		// get all visible son entities
		NESSENGINE_API virtual void __get_visible_entities(RenderablesList& out_list,
			const CameraPtr& camera = NullCamera, bool break_son_nodes = true);

		// get all son entities (all the tiles)
		NESSENGINE_API virtual void __get_all_entities(RenderablesList& out_list, bool breakGroups);

		// update that the tilemap needs update
		NESSENGINE_API virtual void transformations_update();

		// check if this tilemap is really visible
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// get range of tiles that are within the map
		// returns a rectangle. x is first tile i index, w is last tile i index. y is the same for j indexes
		// for example:
		// Rectangle range = map->get_tiles_in_screen();
		// for (int i = range.x; i < range.w; i++) ....
		NESSENGINE_API Rectangle get_tiles_in_screen(const CameraPtr& camera);

		// set source rect from sprite sheet for all tiles, based on position in tilesheet and how many tiles there are
		// for example, if your sprite sheet contains 4x8 animation steps, and you want to set sprite to step (3,2), use it like so:
		//		set_source_from_sprite_sheet(Pointi(3,2), Sizei(4,8));
		// tileIndex: the index of the tile to set inside the sprite sheet (x, y)
		// tilesCount: how many total tiles the sheet has (x, y)
		NESSENGINE_API void set_all_tiles_type(const Pointi& tileIndex, const Sizei& tilesCount);

		// set source rect from sprite sheet for all tiles
		NESSENGINE_API void set_all_tiles_source_rect(const Rectangle& sourceRect);

		// add/remove entities from this node - illegal in tilesmap node!
		NESSENGINE_API virtual void add(const RenderablePtr& object) {throw IllegalAction("Cannot add new entities to tilemap!");}
		NESSENGINE_API virtual void add_first(const RenderablePtr& object) {throw IllegalAction("Cannot add new entities to tilemap!");}
		NESSENGINE_API virtual void remove(const RenderablePtr& object) {throw IllegalAction("Cannot remove entities from tilemap!");}

		// apply the given function to all tiles
		// every call will contain a single tile and its index.
		NESSENGINE_API void apply_to_all(TExecuteOnTiles func);

		// render this tilemap
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

	protected:
		// function to return the first visible tile in screen on x axis
		NESSENGINE_API virtual int get_first_tile_in_screen_x(const Ness::Point& cameraPos);
		// function to return the first visible tile in screen on y axis
		NESSENGINE_API virtual int get_first_tile_in_screen_y(const Ness::Point& cameraPos);
		// function to get amount of tiles in screen on x axis
		NESSENGINE_API virtual int get_tiles_in_screen_x();
		// function to get amount of tiles in screen on y axis
		NESSENGINE_API virtual int get_tiles_in_screen_y();
		// make sure given index are within the tilemap size. 
		// note: i and j may be equal to size.x and size.y, its still count in range
		NESSENGINE_API void put_in_range(int& i, int& j);

	private:
		// arrange a single tile sprite during creation
		NESSENGINE_API void arrange_sprite(const SpritePtr& sprite, const Ness::Pointi& index);
	};

	NESSENGINE_API typedef SharedPtr<TileMap> TileMapPtr;
};