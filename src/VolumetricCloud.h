#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Entity.h"

#ifndef VOLUMETRIC_CLOUD_H
#define VOLUMETRIC_CLOUD_H

namespace Engine
{
	class VolumetricCloud: public Entity
	{
	public:
		VolumetricCloud(int Width, int Height, int Depth);

		void setChanelResolution(int pointsChanel1, int pointsChanel2, int pointsChanel3, int pointsChanel4) {
			voronoiChanelPoints = glm::vec<4, int>(pointsChanel1, pointsChanel2, pointsChanel3, pointsChanel4);
		}

		void GenerateVoronoiPoints();

		void GenerateClouds(bool useGPU = false);

		void Update();

		void Draw(Camera* cam);

		float cutOff = 0.6;
		float cutOffSmothnes = .075f;
		float heightCutoff = 0.15f;

	private:
		int width, heigth, depth, samples;

		Shader* shader = nullptr;
		ComputeShader* cShader = nullptr;

		Model* cube_model = nullptr;

		GLuint texture = 0;

		glm::vec4* volumePoints = nullptr;//Volume discrate points info. Every component represents a chanel

		glm::vec3* voronoiPoints = nullptr;
		int* voronoiPointsChanels = nullptr;

		glm::vec<4, int> voronoiChanelPoints = glm::vec<4, int>(0);

		float random(float min, float max) {
			return min + (((rand() % 100) * 1.f) / 100.f) * max;
		}
	};
}

#endif;
