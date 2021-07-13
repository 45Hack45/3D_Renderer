#include "ParticleSystem.h"
#include <glad.h>
#include <stdlib.h>
#include "logger.h"

#define NUM_PARTICLES 1024*1024 * 1 // total number of particles to move
#define WORK_GROUP_SIZE 128 // # work-items per work-group

float quadVertices[] = {
	// positions     // colors
	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f,  0.05f,  0.0f, 1.0f, 1.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f
	 
};

namespace Engine {

	ParticleSystem::ParticleSystem(glm::vec3 boundBoxMin, glm::vec3 boundBoxMax, float minVelocity, float maxVelocity)
	{

		//--------------------------------------------------------------Compute------------------------
		m_boundBoxMin = boundBoxMin;
		m_boundBoxMax = boundBoxMax;


		//---------------------------------------------------Position
		glGenBuffers(1, &posSSbo);//Create buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct pos), nullptr, GL_STATIC_DRAW);// Initialize memory

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing

		//Map buffer addres to points array 
		points = (struct pos*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct pos), bufMask);

		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			points[i].x =  + random(m_boundBoxMin.x, m_boundBoxMax.x);
			points[i].y = random(m_boundBoxMin.y, m_boundBoxMax.y);
			points[i].z = random(m_boundBoxMin.z, m_boundBoxMax.z);
			points[i].w = 1.f;
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		//---------------------------------------------------Velocity
		glGenBuffers(1, &velSSbo);//Create buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), nullptr, GL_STATIC_DRAW);// Initialize memory

		//Map buffer addres to points array 
		struct vel* vels = (struct vel*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct vel), bufMask);

		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			vels[i].vx = random(-maxVelocity/2, maxVelocity);
			vels[i].vy = random(-maxVelocity/2, maxVelocity);
			vels[i].vz = random(-maxVelocity/2, maxVelocity);
			vels[i].vw = 0.f;
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


		cShader = (ComputeShader*)ShaderManager::Instance()->getShader("TestCompute");


		//--------------------------------------------------------------Render------------------------

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);

		//Create Quad vertices buffer
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);// atribute 0: position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);// atribute 1: color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

		
		glBindBuffer(GL_ARRAY_BUFFER, posSSbo); // this attribute comes from a different vertex buffer
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);// atribute 2: instance_position
		glBindBuffer(GL_ARRAY_BUFFER, 0);
							//atribute pointer	 instances using same value
		glVertexAttribDivisor(2					, 1							); // tell OpenGL this is an instanced vertex attribute.

		shader = ShaderManager::Instance()->getShader("ParticleShader");

		log_printf_info("Particle system initialized");
	}
	void ParticleSystem::Update()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, posSSbo);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, velSSbo);

		cShader->Dispatch(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);//Aply changes
	}
	void ParticleSystem::Draw(Camera* cam)
	{
		//TODO: Render meshes using instancing
		shader->bind();

		shader->setMat4("viewProjection", cam->GetViewProjectionMatrix());

		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NUM_PARTICLES); // NUM_PARTICLES triangles of 6 vertices each
		glBindVertexArray(0);

	}
}
