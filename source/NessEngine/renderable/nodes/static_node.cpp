#include "static_node.h"
#include "../../renderer/renderer.h"
#include "../entities/canvas.h"

namespace Ness
{
	StaticNode::StaticNode(Renderer* renderer, const Sizei& batchSize)
			: Node(renderer), m_batch_size(batchSize) 
	{
		static unsigned int UniqueIds = 0;
		m_unique_id = UniqueIds++;
	}

	// render all the batches
	void StaticNode::render(const CameraPtr& camera)
	{
		// get visible batches range and render them
		Rectangle batches = get_batches_range(camera);
		for (int i = batches.x; i < batches.w; i++)
		{
			for (int j = batches.y; j < batches.h; j++)
			{
				if (m_batches[i][j])
				{
					m_batches[i][j]->render(camera);
				}
			}
		}
	}

	// get all visible batches (canvases)
	void StaticNode::__get_visible_entities(Vector<RenderableAPI*>& out_list, const CameraPtr& camera)
	{
		// get visible batches range and add to out list
		Rectangle batches = get_batches_range(camera);
		for (int i = batches.x; i < batches.w; i++)
		{
			for (int j = batches.y; j < batches.h; j++)
			{
				if (m_batches[i][j])
				{
					out_list.push_back(m_batches[i][j].get());
				}
			}
		}
	}

	// build all the static batches (the matrix of canvases)
	void StaticNode::build(bool removeEntities)
	{
		// first get one big list with all the entities to render (break down son nodes)
		Vector<RenderableAPI*> render_list;
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// check if current entity is a node, and if so, break it
			NodeAPI* currentNode = dynamic_cast<NodeAPI*>(m_entities[i].get());
			if (currentNode)
			{
				currentNode->__get_all_entities(render_list, true);
				continue;
			}

			// if got here it means its an entity, not a node
			render_list.push_back(m_entities[i].get());
		}

		// now render all the entities on the batches
		for (unsigned int i = 0; i < render_list.size(); ++i)
		{
			// get current renderable and make sure it's really an entity
			Entity* curr = dynamic_cast<Entity*>(render_list[i]);
			if (curr == nullptr)
			{
				throw IllegalAction("Static node only support renderables that inherit from 'Entity' base class!");
			}

			// get target rectangle
			curr->get_absolute_transformations();
			Rectangle targetRect = curr->get_last_target_rect();

			// get range of batches this entity is drawn upon
			Rectangle canvases;
			canvases.x = targetRect.x / m_batch_size.x;
			canvases.y = targetRect.y / m_batch_size.y;
			canvases.w = canvases.x + (int)ceil((float)targetRect.w / (float)m_batch_size.x);
			canvases.h = canvases.y + (int)ceil((float)targetRect.h / (float)m_batch_size.y);

			// render this entity on the canvases
			for (int cx = canvases.x; cx <= canvases.w; cx++)
			{
				for (int cy = canvases.y; cy <= canvases.h; cy++)
				{

					// prepare a relative camera
					CameraPtr tempCam = ness_make_ptr<Camera>();
					tempCam->position.x = (float)(cx * m_batch_size.x);
					tempCam->position.y = (float)(cy * m_batch_size.y);

					// first time drawing on this batch? create the canvas!
					if (m_batches[cx][cy].get() == nullptr)
					{
						String batchName("static" + ness_int_to_string(m_unique_id) + "." + ness_int_to_string(cx) + "." + ness_int_to_string(cy));
						m_batches[cx][cy] = ness_make_ptr<Canvas>(this->m_renderer, batchName, m_batch_size);
						m_batches[cx][cy]->set_position(tempCam->position);
						m_batches[cx][cy]->__change_parent(this);
					}

					// set render on current canvas
					m_renderer->set_render_target(m_batches[cx][cy]->get_texture());

					// render the entity on current canvas
					curr->render(tempCam);
				}
			}

			// if need to remove all entities
			if (removeEntities) curr->__change_parent(nullptr);
		}

		// remove all son entities (if needed to)
		if (removeEntities)
		{
			m_entities.clear();
		}

		// reset rendering target
		m_renderer->reset_render_target();
	}

	// get the range of visible batches (canvases)
	Rectangle StaticNode::get_batches_range(const CameraPtr& camera)
	{
		// get absolute position with camera
		SRenderTransformations trans = get_absolute_transformations(); 
		Point pos = trans.position;
		if (camera)
		{
			pos.x -= camera->position.x;
			pos.y -= camera->position.y;
		}

		// get range of batches to return
		Rectangle ret;
		ret.x = (int)(-pos.x / m_batch_size.x);
		ret.y = (int)(-pos.y / m_batch_size.y);
		ret.w = ret.x + (int)((m_renderer->get_target_size().x + m_batch_size.x) / m_batch_size.x) + 1;
		ret.h = ret.y + (int)((m_renderer->get_target_size().y + m_batch_size.y) / m_batch_size.y) + 1;
		return ret;
	}
};