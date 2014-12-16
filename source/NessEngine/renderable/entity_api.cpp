#include "entity_api.h"

namespace Ness
{
	bool EntityAPI::touch_point(const Pointf& pos) const
	{
		const Rectangle& rect = get_last_target_rect();
		return (pos.x >= rect.x && pos.y >= rect.y && pos.x <= rect.x + rect.w && pos.y <= rect.y + rect.h);
	}
}