#include "RenderPass_Transparent.h"

namespace Engine
{
	const int _PI = 3.14159265359;

	const FrameBuffer* RenderPass_Transparent::RenderPass(const FrameBuffer* in_fbo)
	{
		/*glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(1, 1, 1, 1);
		glClear(GL_DEPTH_BUFFER_BIT);*/

		for (auto entity : m_scene->entities) {

			if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
				continue;

			if (!entity->meshRenderer.m_material->isTransparent)//evaluate the entity
				continue;

			Shader* shader = entity->meshRenderer.m_material->m_shader;
			shader->bind();

			glm::mat4 model = entity->transform.globalSpace();

			shader->setMat4("model", model);

			sendProjectionInfo2Shader(shader, cam);
			entity->meshRenderer.m_material->sendMaterial2Shader();

			sendLightInfo2Shader(shader, m_scene->m_pointLights, m_scene->m_spotLights, m_scene->m_directionalLight);
			sendCascadeShadowMapInfo2Shader(shader, in_fbo);

			entity->meshRenderer.Render();
		}

		return in_fbo;
	}

	void RenderPass_Transparent::RenderPassDebugGUI()
	{

	}

	void RenderPass_Transparent::sendProjectionInfo2Shader(Shader* shader, Camera* cam)
	{
		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 projection = glm::mat4(1.f);

		view = cam->GetViewMatrix();

		float near = .01f;
		float far = 1000.0f;

		projection *= cam->GetProjectionMatrix();

		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVector("viewPos", cam->Position);
	}

	void RenderPass_Transparent::sendCascadeShadowMapInfo2Shader(Shader* shader, const FrameBuffer* shadow_fbo)
	{
		shader->setInt("shadowMap", 15);//set shadowMap sampler to active texture 15
		shadow_fbo->bindDepthTexture(15);//bind depth texture to active texture 15

		shader->setMat4("shadowMatrices[0]", auxCam_shadowmap[0].GetViewProjectionMatrix());
		shader->setMat4("shadowMatrices[1]", auxCam_shadowmap[1].GetViewProjectionMatrix());
		shader->setMat4("shadowMatrices[2]", auxCam_shadowmap[2].GetViewProjectionMatrix());
		shader->setMat4("shadowMatrices[3]", auxCam_shadowmap[3].GetViewProjectionMatrix());

		shader->setInt("numCascades", 4);

		shader->setFloat("shadowBias", shadowBias * .00001f);
		shader->setFloat("shadowBiasMax", shadowBiasMax * .00001f);
	}

	void RenderPass_Transparent::sendLightInfo2Shader(Shader* shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight)
	{
		//Setting directional light
		shader->setFloat("lightsrc_directional_intensity", dirLight.getIntensity());
		shader->setVector("lightsrc_directional_color", dirLight.getColor().getColorVec());
		shader->setVector("lightsrc_directional_direction", dirLight.getDirection());

		shader->setFloat("ambientLight", ambientLight);
		shader->setVector("ambientColor", glm::vec4(1, 1, 1, 1));

		for (int i = 0; i < pointLights.size(); i++)
		{
			std::string pointsi = "pointLights[";
			pointsi += std::to_string(i);
			pointsi += "].";

			//std::cout << (pointsi + "color") << endl;
			/*if(i==0)
				std::cout << pointLights[i].m_intensity<< endl;*/

			shader->setVector(pointsi + "color", pointLights[i].getColor().getColorVec());
			shader->setVector(pointsi + "position", pointLights[i].getPosition());
			shader->setFloat(pointsi + "intensity", pointLights[i].m_intensity);
			shader->setFloat(pointsi + "linear", pointLights[i].m_linear);
			shader->setFloat(pointsi + "quadratic", pointLights[i].m_quadratic);
		}

		for (int i = 0; i < spotLights.size(); i++)
		{
			std::string pointsi = "spotLights[";
			pointsi += std::to_string(i);
			pointsi += "].";

			//std::cout << (pointsi + "color") << endl;
			/*if(i==0)
				std::cout << pointLights[i].m_intensity<< endl;*/

			shader->setVector(pointsi + "color", spotLights[i].getColor().getColorVec());
			shader->setVector(pointsi + "position", spotLights[i].m_pos);
			shader->setVector(pointsi + "direction", normalize(spotLights[i].m_dir));
			shader->setFloat(pointsi + "intensity", spotLights[i].m_intensity);
			shader->setFloat(pointsi + "linear", spotLights[i].m_linear);
			shader->setFloat(pointsi + "quadratic", spotLights[i].m_quadratic);
			shader->setFloat(pointsi + "angle", std::cos(spotLights[i].m_angle * _PI / 180));
			shader->setFloat(pointsi + "outerAngle", std::cos((spotLights[i].m_angle + spotLights[i].m_outerAngle) * _PI / 180));
		}

		shader->setInt("nPointLights", pointLights.size());
		shader->setInt("nSpotLights", spotLights.size());
	}
}
