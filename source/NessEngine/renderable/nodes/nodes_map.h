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
* A renderable tilemap, consist of a matrix of nodes instead of sprites.
* have special optimizations for culling and efficient rendering
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"
#include "../transformable_api.h"
#include "../node_api.h"
#include "../../basic_types/containers.h"

namespace Ness
{
	// callback function to run on all nodes
	NESSENGINE_API typedef void (*TExecuteOnNodes)(const Ness::Pointi& index, const Ness::NodeAPIPtr& node);

	// callback function to create custom sprite types for the tilemap
	NESSENGINE_API typedef NodeAPIPtr (*TCreateNodes)(const Pointi& index);

	/* 
	* NodesMap is a special node that creates a grid of son nodes, mostly useable to represent a more complicated tilemap
	*/
	class NodesMap : public NodeAPI
	{
	protected:
		Sizei										m_size;						// size of the tilemap
		Size										m_nodes_distance;			// distance between nodes
		Size										m_node_size;				// size of a single node in pixels
		Vector< Vector<NodeAPIPtr> >				m_nodes;					// the nodes matrix
		SRenderTransformations						m_absolute_transformations;	// absolute transformations of this nodes tilemap
		Sizei										m_extra_tiles_factor;		// extra tiles to render (count in screen) on eatch side of x and y axis

	public:

		// create the nodesmap
		// mapSize - how many nodes there are on rows and columns
		// nodesSize - estimated size of a single node, required for culling. 
		//				if you put entities outside the given node size, they might be culled out when rendered.
		// nodesDistance - the distance between nodes. if zero, will just use nodes size
		// createNodesFunction - if provided, a function that creates the node objects for this nodesmap. you can use it to create nodemap with
		//							special type of nodes which are not the basic scene node.
		NESSENGINE_API NodesMap(Renderer* renderer, const Sizei& mapSize, const Size& nodesSize, const Size& nodesDistance = Size::ZERO, TCreateNodes createNodesFunction = nullptr);

		// return the absolute transformations of this tilemap
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// return tilemap params
		NESSENGINE_API inline const Sizei& get_map_size() const {return m_size;}
		NESSENGINE_API inline const Size& get_nodes_distance() const {return m_nodes_distance;}
		NESSENGINE_API inline const Size& get_nodes_size() const {return m_node_size;}

		// set extra tiles to render in screen for x and y axis.
		// what is it for? if your normal tiles size is 32x32 but sometimes you have tiles of 32x64, you can set it to (0, 1), meaning you
		// will count additional tile from top and bottom when rendering the tiles in screen
		NESSENGINE_API inline void set_extra_nodes_in_screen(const Ness::Sizei& extra) {m_extra_tiles_factor = extra;}

		// return if need transformations udpate (always false for tilemap)
		NESSENGINE_API virtual bool need_transformations_update() {return false;}

		// get a specific node by index, for any type of node (up to you to make the casting)
		NESSENGINE_API inline NodeAPIPtr& get_node_any(const Pointi& index) {return m_nodes[index.x][index.y];}

		// get a specific node, assuming it's a basic scene node type.
		// If your nodes map does not use regular scenes node don't use this, you'll get null
		NESSENGINE_API inline NodePtr get_node(const Pointi& index) {return ness_ptr_cast<Node>(m_nodes[index.x][index.y]);}

		// direct access to son entities (note: son entities are in vector so efficiecny is alright here)
		NESSENGINE_API virtual unsigned int get_sons_count() const {return m_size.x * m_size.y;}
		NESSENGINE_API virtual RenderablePtr get_son(unsigned int index) {return m_nodes[index % m_size.x][index / m_size.x];}

		// get a specific node by position
		// return empty if out of range
		NESSENGINE_API virtual NodeAPIPtr& get_node_by_position_any(const Point& position);

		// return node position from a given index
		NESSENGINE_API virtual Point get_position_from_index(const Pointi& index) const;

		// get a specific node by position
		// If your nodes map does not use regular scenes node don't use this, you'll get null
		// return empty if out of range or if your nodes map does not use regular scene nodes
		NESSENGINE_API inline NodePtr get_node_by_position(const Point& position) {return ness_ptr_cast<Node>(get_node_by_position_any(position));}

		// return index of tile from position
		NESSENGINE_API virtual Pointi get_index_from_position(const Point& position);

		// get all visible son nodes and entities
		NESSENGINE_API virtual void __get_visible_entities(Vector<RenderableAPI*>& out_list, const CameraPtr& camera = NullCamera);

		// get all son entities (all the tiles)
		NESSENGINE_API virtual void __get_all_entities(Vector<RenderableAPI*>& out_list, bool breakGroups);

		// update that the tilemap needs update
		NESSENGINE_API virtual void transformations_update();

		// check if this tilemap is really visible
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// get range of tiles that are within the map
		// returns a rectangle. x is first node i index, w is last node i index. y is the same for j indexes
		// for example:
		// Rectangle range = map->get_nodes_in_screen();
		// for (int i = range.x; i < range.w; i++) ....
		NESSENGINE_API Rectangle get_nodes_in_screen(const CameraPtr& camera);

		// add/remove entities from this node - illegal in tilesmap node!
		NESSENGINE_API virtual void add(const RenderablePtr& object) {throw IllegalAction("Cannot add new entities to tilemap!");}
		NESSENGINE_API virtual void add_first(const RenderablePtr& object) {throw IllegalAction("Cannot add new entities to tilemap!");}
		NESSENGINE_API virtual void remove(const RenderablePtr& object) {throw IllegalAction("Cannot remove entities from tilemap!");}

		// apply the given function to all tiles
		// every call will contain a single tile and its index.
		NESSENGINE_API void apply_to_all(TExecuteOnNodes func);

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
		// make sure given index are within the tilemap size
		NESSENGINE_API void put_in_range(int& i, int& j);

	private:
		// arrange a single tile sprite during creation
		NESSENGINE_API void arrange_node(const NodeAPIPtr& node, const Ness::Pointi& index);
	};

	NESSENGINE_API typedef SharedPtr<NodesMap> NodesMapPtr;
};