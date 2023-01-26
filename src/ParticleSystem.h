#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "ShaderManager.hpp"
#include "Camera.h"

namespace Engine
{
	class ParticleSystem
	{
	public:

		ParticleSystem(glm::vec3 boundBoxMin, glm::vec3 boundBoxMax, float minVelocity, float maxVelocity);

		void Update();

		void Draw(Camera* cam);

	private:

		ComputeShader*	cShader;
		Shader*			shader;

		glm::vec3 m_boundBoxMin;
		glm::vec3 m_boundBoxMax;

		// need to do the following for both position, velocity, and colors of the particles:
		GLuint posSSbo;
		GLuint velSSbo;
		GLuint colSSbo;

		GLuint quadVAO, quadVBO;

		struct pos
		{
			float x, y, z, w; // positions
		};
		struct vel
		{
			float vx, vy, vz, vw; // velocities
		};
		struct color
		{
			float r, g, b, a; // colors
		};

		struct pos* points;

		float random(float min, float max) {
			return min + (((rand() % 100) * 1.f) / 100.f) * max;
		}
	};
}

