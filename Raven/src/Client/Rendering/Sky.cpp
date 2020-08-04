#include "Sky.h"
#include <GFX/RenderCore.h>
namespace Minecraft::Rendering {
	Sky::Sky()
	{
		Update(0);
		float zDist = 2.2f;

		for (int i = 0; i < 6; i++) {
			face[i].color = {
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
			};
			face[i].indices = {
				3, 2, 1, 1, 0, 3
			};

			face[i].uv = {
				0, 0,
				1, 0,
				1, 1,
				0, 1
			};
		}

		face[0].position = {
			zDist, zDist, -zDist,
			zDist, zDist, zDist,
			zDist, -zDist, zDist,
			zDist, -zDist, -zDist,
		};


		face[1].position = {
			-zDist, zDist, zDist,
			-zDist, zDist, -zDist,
			-zDist, -zDist, -zDist,
			-zDist, -zDist, zDist,
		};


		face[2].position = {
			zDist, zDist, zDist,
			-zDist, zDist, zDist,
			-zDist, -zDist, zDist,
			zDist, -zDist, zDist,
		};

		face[3].position = {
			zDist, zDist,	-zDist,
			zDist, -zDist,	-zDist,
			-zDist, -zDist, -zDist,
			-zDist, zDist,	-zDist,
		};

		face[4].position = {
			zDist, -zDist, -zDist,
			zDist, -zDist, zDist,
			-zDist, -zDist, zDist,
			-zDist, -zDist, -zDist,
		};

		face[5].position = {
		   -zDist,	zDist, -zDist,
		   -zDist,	zDist,	zDist,
			zDist,	zDist,	zDist,
			zDist,	zDist, -zDist,
		};

		for (int i = 0; i < 6; i++) {
			faces[i].addData(face[i]);
		}
		Update(0);
	}

	Sky::~Sky()
	{

	}

	glm::vec3 getColor(float dayTime, float vertex);

	void Sky::Update(uint64_t timeOfDay)
	{
		int i = 0;

		glm::vec3 v0[4];

		float skyColorBoost = 0.15f;
		for (int i = 0; i < 4; i++) {
			v0[i] = getColor(timeOfDay % 24000, i);
			v0[i].r = v0[i].r * (1.0f + skyColorBoost) - (v0[i].g * skyColorBoost / 2.0f) - (v0[i].b * skyColorBoost / 2.0f);
			if (v0[i].r > 1.0) {
				v0[i].r = 1.0;
			}
			v0[i].g = v0[i].g * (1.0f + skyColorBoost) - (v0[i].r * skyColorBoost / 2.0f) - (v0[i].b * skyColorBoost / 2.0f);
			if (v0[i].g > 1.0) {
				v0[i].g = 1.0;
			}
			v0[i].b = v0[i].b * (1.0f + skyColorBoost) - (v0[i].r * skyColorBoost / 2.0f) - (v0[i].g * skyColorBoost / 2.0f);
			if (v0[i].b > 1.0) {
				v0[i].b = 1.0;
			}

		}

		face[0].color = {
				v0[0].x, v0[0].y, v0[0].z, 1.0f,
				v0[0].x, v0[0].y, v0[0].z, 1.0f,
				v0[1].x, v0[1].y, v0[1].z, 1.0f,
				v0[1].x, v0[1].y, v0[1].z, 1.0f,
		};
		face[1].color = {
				v0[2].x, v0[2].y, v0[2].z, 1.0f,
				v0[2].x, v0[2].y, v0[2].z, 1.0f,
				v0[3].x, v0[3].y, v0[3].z, 1.0f,
				v0[3].x, v0[3].y, v0[3].z, 1.0f,
		};
		
		face[2].color = {
				v0[2].x, v0[2].y, v0[2].z, 1.0f,
				v0[0].x, v0[0].y, v0[0].z, 1.0f,
				v0[3].x, v0[3].y, v0[3].z, 1.0f,
				v0[1].x, v0[1].y, v0[1].z, 1.0f,
		};

		face[3].color = {
				v0[2].x, v0[2].y, v0[2].z, 1.0f,
				v0[1].x, v0[1].y, v0[1].z, 1.0f,
				v0[3].x, v0[3].y, v0[3].z, 1.0f,
				v0[0].x, v0[0].y, v0[0].z, 1.0f,
		};
		
		

		face[4].color = {
				v0[1].x, v0[1].y, v0[1].z, 1.0f,
				v0[1].x, v0[1].y, v0[1].z, 1.0f,
				v0[3].x, v0[3].y, v0[3].z, 1.0f,
				v0[3].x, v0[3].y, v0[3].z, 1.0f,
		};

		face[5].color = {
				v0[0].x, v0[0].y, v0[0].z, 1.0f,
				v0[0].x, v0[0].y, v0[0].z, 1.0f,
				v0[2].x, v0[2].y, v0[2].z, 1.0f,
				v0[2].x, v0[2].y, v0[2].z, 1.0f,
		};

		for (int i = 0; i < 6; i++) {
			faces[i].addData(face[i]);
			faces[i].noTex = true;
		}
	}

	void Sky::Draw(GFX::Render3D::Camera* cam)
	{
#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGuDisable(GU_TEXTURE_2D);
		sceGuEnable(GU_BLEND);
		sceGuDisable(GU_DEPTH_TEST);
#else
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
#endif

		glm::vec3 pos = cam->pos;
		cam->pos = { 0, 0, 0 };

		cam->update();
		GFX::g_RenderCore->set3DMode();

		for (int i = 0; i < 6; i++) {
			faces[i].draw();
		}

		cam->pos = pos;

		cam->update();
		GFX::g_RenderCore->set3DMode();

#if CURRENT_PLATFORM == PLATFORM_PSP
		sceGuEnable(GU_TEXTURE_2D);
		sceGuEnable(GU_BLEND);
		sceGuDisable(GU_ENABLE_TEST);
#else
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
#endif
	}



	glm::vec3 zenithColorDay = glm::vec3(0.5, 0.7, 1.0);
	glm::vec3 horizonColorDay = glm::vec3(0.85, 0.91, 1.0);

	glm::vec3 zenithColorNight = glm::vec3(0.06, 0.05, 0.09);
	glm::vec3 horizonColorNight = glm::vec3(0.07, 0.06, 0.1);

	glm::vec3 zenithColorSunRise = glm::vec3(0.45, 0.5, 0.7);
	glm::vec3 horizonColorSunRise = glm::vec3(0.9, 0.45, 0.15);
	glm::vec3 zenithColorMoonRise = glm::vec3(0.45, 0.5, 0.7);
	glm::vec3 horizonColorMoonRise = glm::vec3(0.5, 0.45, 0.6);

	glm::vec3 getColor(float dayTime, float vertex) {
		if (vertex == 0) {
			if (dayTime < 2000) { //6am - 9am sun gets brighter
				return glm::mix(zenithColorMoonRise, zenithColorDay, dayTime / 2000);
			}
			if (dayTime >= 2000 && dayTime < 10000) { //9am - 3pm sun is brightest
				return zenithColorDay;
			}
			if (dayTime >= 10000 && dayTime < 12000) { //3pm - 6pm sun gets dimmer
				return glm::mix(zenithColorDay, zenithColorSunRise, (dayTime - 10000) / 2000);
			}
			if (dayTime >= 12000 && dayTime < 14000) { //6pm - 9pm sun light fades
				return glm::mix(zenithColorSunRise, zenithColorNight, (dayTime - 12000) / 2000);
			}
			if (dayTime >= 14000 && dayTime < 22000) {//9pm - 3am is night
				return zenithColorNight;
			}
			if (dayTime >= 22000 && dayTime < 24000) {
				return glm::mix(zenithColorNight, zenithColorMoonRise, (dayTime - 22000) / 2000);
			}
		}
		else if (vertex == 1) {
			if (dayTime < 2000) { //6am - 9am sun gets brighter
				return glm::mix(horizonColorMoonRise, horizonColorDay, dayTime / 2000);
			}
			if (dayTime >= 2000 && dayTime < 10000) { //9am - 3pm sun is brightest
				return horizonColorDay;
			}
			if (dayTime >= 10000 && dayTime < 12000) { //3pm - 6pm sun gets dimmer
				return glm::mix(horizonColorDay, horizonColorSunRise, (dayTime - 10000) / 2000);
			}
			if (dayTime >= 12000 && dayTime < 14000) { //6pm - 9pm sun light fades
				return glm::mix(horizonColorSunRise, horizonColorNight, (dayTime - 12000) / 2000);
			}
			if (dayTime >= 14000 && dayTime < 22000) {//9pm - 3am is night
				return horizonColorNight;
			}
			if (dayTime >= 22000 && dayTime < 24000) {
				return glm::mix(horizonColorNight, horizonColorMoonRise, (dayTime - 22000) / 2000);
			}
		}
		else if (vertex == 2) {
			if (dayTime < 2000) { //6am - 9am sun gets brighter
				return glm::mix(zenithColorSunRise, zenithColorDay, dayTime / 2000);
			}
			if (dayTime >= 2000 && dayTime < 10000) { //9am - 3pm sun is brightest
				return zenithColorDay;
			}
			if (dayTime >= 10000 && dayTime < 12000) { //3pm - 6pm sun gets dimmer
				return glm::mix(zenithColorDay, zenithColorMoonRise, (dayTime - 10000) / 2000);
			}
			if (dayTime >= 12000 && dayTime < 14000) { //6pm - 9pm sun light fades
				return glm::mix(zenithColorMoonRise, zenithColorNight, (dayTime - 12000) / 2000);
			}
			if (dayTime >= 14000 && dayTime < 22000) {//9pm - 3am is night
				return zenithColorNight;
			}
			if (dayTime >= 22000 && dayTime < 24000) {
				return glm::mix(zenithColorNight, zenithColorSunRise, (dayTime - 22000) / 2000);
			}
		}
		else if (vertex == 3) {
			if (dayTime < 2000) { //6am - 9am sun gets brighter
				return glm::mix(horizonColorSunRise, horizonColorDay, dayTime / 2000);
			}
			if (dayTime >= 2000 && dayTime < 10000) { //9am - 3pm sun is brightest
				return horizonColorDay;
			}
			if (dayTime >= 10000 && dayTime < 12000) { //3pm - 6pm sun gets dimmer
				return glm::mix(horizonColorDay, horizonColorMoonRise, (dayTime - 10000) / 2000);
			}
			if (dayTime >= 12000 && dayTime < 14000) { //6pm - 9pm sun light fades
				return glm::mix(horizonColorMoonRise, horizonColorNight, (dayTime - 12000) / 2000);
			}
			if (dayTime >= 14000 && dayTime < 22000) {//9pm - 3am is night
				return horizonColorNight;
			}
			if (dayTime >= 22000 && dayTime < 24000) {
				return glm::mix(horizonColorNight, horizonColorSunRise, (dayTime - 22000) / 2000);
			}
		}
		return glm::vec3(0, 0, 0);
	}

}
