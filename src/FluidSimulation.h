#pragma once
#include "Shader.h"
#include "Camera.h"

namespace Engine
{
	class FluidSimulation
	{
	public:
		FluidSimulation(int widht, int height, int depth);

		void Update();

		void Draw(Camera* cam);
	private:
		ComputeShader* cShader;
		GLuint velTex, nVelTex;
		GLuint presureTex, nPresureTex;
		int width, heigth, depth;
		int samples = 0;

		struct vel
		{
			float vx, vy, vz, vw; // velocities
		};

		vel* oldVel, * newVel;
		float* oldPresure, * newPresure;

		float random(float min, float max) {
			return min + (((rand() % 100) * 1.f) / 100.f) * max;
		}
	};
}
