#include "sprite3d.h"

Sprite3d::Sprite3d(Ness::LightNodePtr lightNode, const Ness::String& TextureFile) 
	: Sprite(lightNode->renderer(), TextureFile), m_light_node(lightNode)
{
	m_light_left = m_renderer->resources().get_texture(TextureFile + ".left.png");
	m_light_right = m_renderer->resources().get_texture(TextureFile + ".right.png");
	m_light_front = m_renderer->resources().get_texture(TextureFile + ".bottom.png");
	set_blend_mode(Ness::BLEND_MODE_BLEND);
	set_anchor(Ness::Point(0.5f, 0.5f));
	m_render_diffuse = true;
}

void Sprite3d::render(const Ness::CameraPtr& camera)
{
	m_last_camera = camera;
	Ness::Sprite::render(camera);
}

void Sprite3d::do_render(const Ness::Rectangle& target, const Ness::SRenderTransformations& transformations)
{

	// first render black figure of this texture
	m_renderer->blit(m_texture, &m_source_rect, target, Ness::BLEND_MODE_BLEND, m_light_node->get_ambient_color(), transformations.rotation, m_anchor);

	// get some params we need: list of lights in screen, factor, and absolute position
	Ness::Containers::Vector<Ness::LightPtr> lights;
	m_light_node->get_lights_in_screen(lights, m_last_camera);
	const float VectorFactor = 1.75f;

	// get self position  (replace y with z)
	Ness::Point Position = get_absolute_position();
	Position.y = get_absolute_zindex();

	// now for every light in the lighting node, render additive layer based on light direction and distance
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		Ness::LightPtr& curr = lights[i];

		// get light position (replace y with z)
		Ness::Point LightPos = curr->get_absolute_position();
		LightPos.y = curr->get_absolute_zindex();

		// get size color and distance
		Ness::Point LightHalfSize = curr->get_absolute_size_const() * 0.5f;
		Ness::Color CurrColor = curr->get_color();
		float distance = Position.distance(LightPos);
		Ness::Point vector = (Position - LightPos);
		vector.normalize();
		vector.abs();

		// if this light is left to this object most right side, check lighting from left side:
		if (LightPos.x < Position.x)
		{
			float factor = LightHalfSize.x;
			CurrColor.a = ((factor) - distance) / factor;
			CurrColor.a = CurrColor.a > 1.0f ? 1.0f : CurrColor.a;
			if (CurrColor.a > 0.0f)
			{
				CurrColor.a *= vector.x * VectorFactor;
				CurrColor.a = CurrColor.a > 1.0f ? 1.0f : CurrColor.a;
				m_renderer->blit(m_light_left, &m_source_rect, target, Ness::BLEND_MODE_ADD, CurrColor, transformations.rotation, m_anchor);
			}
		}
		// if this light is right to this object most left side, check lighting from right side:
		if (LightPos.x > Position.x)
		{
			float factor = LightHalfSize.x;
			CurrColor.a = ((factor) - distance) / factor;
			CurrColor.a = CurrColor.a > 1.0f ? 1.0f : CurrColor.a;
			if (CurrColor.a > 0.0f)
			{
				CurrColor.a *= vector.x * VectorFactor;
				CurrColor.a = CurrColor.a > 1.0f ? 1.0f : CurrColor.a;
				m_renderer->blit(m_light_right, &m_source_rect, target, Ness::BLEND_MODE_ADD, CurrColor, transformations.rotation, m_anchor);
			}
		}
		// if this light is below this object top, check lighting from bottom:
		if (LightPos.y > Position.y)
		{
			float factor = LightHalfSize.y;
			CurrColor.a = ((factor) - distance) / factor;
			CurrColor.a = CurrColor.a > 1.0f ? 1.0f : CurrColor.a;
			if (CurrColor.a > 0.0f)
			{
				CurrColor.a *= vector.y * VectorFactor;
				CurrColor.a = CurrColor.a > 1.0f ? 1.0f : CurrColor.a;
				m_renderer->blit(m_light_front, &m_source_rect, target, Ness::BLEND_MODE_ADD, CurrColor, transformations.rotation, m_anchor);
			}
		}
	}

	// finally, render diffuse color
	if (m_render_diffuse)
		m_renderer->blit(m_texture, &m_source_rect, target, Ness::BLEND_MODE_MOD, transformations.color, transformations.rotation, m_anchor);
}