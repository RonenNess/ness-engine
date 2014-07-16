#include "shapes.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	RectangleShape::RectangleShape(Renderer* renderer) : Entity(renderer), m_is_filled(true)
	{
	}

	void RectangleShape::do_render(const Rectangle& target, const SRenderTransformations& transformations)
	{
		m_renderer->draw_rect(target, transformations.color, m_is_filled, transformations.blend);
	}
};