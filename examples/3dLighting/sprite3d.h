#include <NessEngine.h>

// our customized sprite with 3d lightings effect
class Sprite3d : public Ness::Sprite
{
private:
	Ness::ManagedResources::ManagedTexturePtr		m_light_top;
	Ness::ManagedResources::ManagedTexturePtr		m_light_bottom;
	Ness::ManagedResources::ManagedTexturePtr		m_light_left;
	Ness::ManagedResources::ManagedTexturePtr		m_light_right;
	Ness::LightNodePtr								m_light_node;
	Ness::CameraPtr									m_last_camera;

public:

	Sprite3d(Ness::LightNodePtr lightNode, const std::string& TextureFile);

	// override the render function just to store the camera before do_render (we need it inside)
	virtual void render(const Ness::CameraPtr& camera = Ness::NullCamera);

	// do the 3d-like rendering
	virtual void Sprite3d::do_render(const Ness::Rectangle& target, const Ness::SRenderTransformations& transformations);
};