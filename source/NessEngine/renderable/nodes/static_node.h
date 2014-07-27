/**
* A static node is a node that renders everything on a matrix of canvases (called batches), and then renders them.
* this can greatly optimize things that are static, like tilemap or background stuff, by reducing the amounts of drawings
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"

namespace Ness
{
	class StaticNode : public Node
	{
	private:
		UnorderedMap< int, UnorderedMap<int, CanvasPtr> >	m_batches;
		Sizei												m_batch_size;
		unsigned int										m_unique_id;

	public:
		// create the static node. be sure to call 'build' after setting all the objects to generate the batches.
		// batchSize - the size of every single batch (canvas) in this static node.
		NESSENGINE_API StaticNode(Renderer* renderer, const Sizei& batchSize = Sizei(512, 512));

		// build all the static batches (the matrix of canvases)
		NESSENGINE_API void build();

		// render the static batches
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

		// get a list with ALL the son entities that are currently visible in screen
		NESSENGINE_API virtual void __get_visible_entities(Vector<RenderableAPI*>& out_list, const CameraPtr& camera = NullCamera);

	protected:
		// return the range of visible batches
		NESSENGINE_API Rectangle get_batches_range(const CameraPtr& camera);
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<StaticNode> StaticNodePtr;
};