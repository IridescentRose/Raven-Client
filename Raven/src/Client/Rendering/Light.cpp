#include "Light.h"
#include <GFX/RenderCore.h>

namespace Minecraft::Rendering {
	SkyLight::SkyLight(unsigned int tex)
	{
		degrees = 0.0f;
		texture = tex;

		mesh.color = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		mesh.indices = {
			3, 2, 1, 1, 0, 3
		};

		mesh.uv = {
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		};

		mesh.position = {
			-128.0f, -128.0f, 0.0f,
			128.0f, -128.0f, 0.0f,
			128.0f, 128.0f, 0.0f,
			-128.0f, 128.0f, 0.0f,
		};

		face.addData(mesh);

	}

	SkyLight::~SkyLight()
	{
	}

	void SkyLight::Update(uint64_t timeOfDay)
	{
		degrees = (1.0f - timeOfDay / 24000.0f) * 360.0f;
	}

	void SkyLight::Draw(GFX::Render3D::Camera* cam)
	{
		GFX::g_TextureManager->bindTex(texture);

		glm::vec3 pos = cam->pos;
		cam->pos = { 0, 0, 0 };

		cam->update();
		GFX::g_RenderCore->set3DMode();

		cam->update();
		GFX::g_RenderCore->set3DMode();

		GFX::pushMatrix();
		GFX::rotateModelMatrix({ 0.0f, 270.0f, 0.0f });
		GFX::pushMatrix();
		GFX::rotateModelMatrix({ degrees, 0.0f, 0.0f });
		GFX::translateModelMatrix({ 0.0f, 0.0f, 256.0f });

		face.draw();

		GFX::popMatrix();
		GFX::popMatrix();

		cam->pos = pos;
		cam->update();
		GFX::g_RenderCore->set3DMode();

	}

}