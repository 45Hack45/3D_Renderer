#include "FluidSimulation.h"
#include "ShaderManager.hpp"

#define WORK_GROUP_SIZE 8 // # work-items per work-group

namespace Engine{

	FluidSimulation::FluidSimulation(int Width, int Heigth, int Depth)
	{
		width = Width;
		heigth = Heigth;
		depth = Depth;
		samples = width * heigth * depth;

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing

		//---------------------------------------------------Velocity Old
		glGenTextures(1, &velTex);
		glBindTexture(GL_TEXTURE_3D, velTex);

		oldVel = new vel[samples];

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, heigth, depth, 0, GL_RGBA, GL_HALF_FLOAT, oldVel);

		glBindTexture(GL_TEXTURE_3D, 0);

		//---------------------------------------------------Velocity New
		glGenTextures(1, &nVelTex);
		glBindTexture(GL_TEXTURE_3D, nVelTex);

		newVel = new vel[samples];

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, heigth, depth, 0, GL_RGBA, GL_HALF_FLOAT, newVel);

		glBindTexture(GL_TEXTURE_3D, 0);

		//---------------------------------------------------Presure Old

		//---------------------------------------------------Presure New


		cShader = (ComputeShader*)ShaderManager::Instance()->getShader("VolumetricWaterCompute");

		log_printf_info("Fluid Simulation initialized");
	}

	void FluidSimulation::Update()
	{

		cShader->Dispatch(width / WORK_GROUP_SIZE, heigth / WORK_GROUP_SIZE, width / WORK_GROUP_SIZE);

		//Apply changes
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	void FluidSimulation::Draw(Camera* cam)
	{
	}
}
