/*
* For box2d debug rendering
*
* Athor: Ronen Ness
* Since: 10/14
*/

#pragma once

#include "box2d_converter.h"

class Box2dDebugDraw : public b2Draw
  {
	private:
		Ness::Renderer* m_renderer;
		Ness::CameraPtr m_camera;

	public:
		Box2dDebugDraw(Ness::Renderer* renderer, Ness::CameraPtr camera) 
		{
			m_renderer = renderer;
			if (camera)
			{
				m_camera = camera;
			}
			else
			{
				m_camera = m_renderer->create_camera();
			}
		}

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
		{
			for (int i = 0; i <= vertexCount-2; i++)
			{
				m_renderer->draw_line(	Box2dToNess::point(vertices[i]) - m_camera->position, 
										Box2dToNess::point(vertices[i+1]) - m_camera->position, 
										Box2dToNess::color(color));
			}
			m_renderer->draw_line(	Box2dToNess::point(vertices[0]) - m_camera->position, 
										Box2dToNess::point(vertices[vertexCount-1]) - m_camera->position, 
										Box2dToNess::color(color));
		}

		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
		{
			DrawPolygon(vertices, vertexCount, color);
		}

		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) 
		{
			m_renderer->draw_circle(Box2dToNess::point(center) - m_camera->position, Box2dToNess::scalar(radius), Box2dToNess::color(color));
		}

		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) 
		{
			DrawCircle(center, radius, color);
		}

		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
		void DrawTransform(const b2Transform& xf) {}
  };