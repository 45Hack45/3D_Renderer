#include "RenderPass_ShadowMap.h"

namespace Engine
{
	const FrameBuffer* RenderPass_ShadowMap::RenderPass(const FrameBuffer* in_fbo)
	{
		shadow_fbo->bind(true);

		LightSource_Spot* spotLight = &scene->m_spotLights[0];
 		LightSource_Directional* dirlight = &scene->m_directionalLight;

		dirlight->updateDirection();
		glm::vec3 dir = -glm::normalize(dirlight->getDirection());

		glm::vec3 pos;

		pos = cam->Position + cam->Front * camDirOffsetFactor;
		pos.y = 0;
		pos +=  -dir * auxCamDistance;

		auxCams[0].Position = pos;
		auxCams[1].Position = pos;
		auxCams[2].Position = pos;
		auxCams[3].Position = pos;

		auxCams[0].setFront(dir);
		auxCams[1].setFront(dir);
		auxCams[2].setFront(dir);
		auxCams[3].setFront(dir);

		RenderCascadeShadowmaps(shadowmapShader, pos);//Render

		return shadow_fbo;
	}

	void RenderPass_ShadowMap::RenderPassDebugGUI()
	{
		if (ImGui::Begin("Shadowmap")) {
			ImGui::DragFloat("Horizontal", &auxCams[0].ortoFrustrum_Horizontal);
			ImGui::DragFloat("Vertical", &auxCams[0].ortoFrustrum_Vertical);

			ImGui::DragFloat("Near", &auxCams[0].near);
			ImGui::DragFloat("Far", &auxCams[0].far);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::DragFloat("AuxCam Distance", &auxCamDistance);
			ImGui::DragFloat("camDirOffsetFactor", &camDirOffsetFactor);
		}
		ImGui::End();
	}

	void RenderPass_ShadowMap::RenderCascadeShadowmaps(Shader* shader, glm::vec3 lightPos) {
		glClearColor(1, 1, 1, 1);
		glClearDepth(1.f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		float res = 1024 * shadowMap_Resolution;

		shadowmapShader->bind();

		shader->setVector("lightPos", lightPos);

		shader->setFloat("far_plane", auxCams[0].far);
		glViewport(0, 0, res, res);
		renderPass(&auxCams[0]);

		shader->setFloat("far_plane", auxCams[1].far);
		glViewport(res, 0, res, res);
		renderPass(&auxCams[1]);

		shader->setFloat("far_plane", auxCams[2].far);
		glViewport(res * 2, 0, res, res);
		renderPass(&auxCams[2]);

		shader->setFloat("far_plane", auxCams[0].far);
		glViewport(res * 3, 0, res, res);
		renderPass(&auxCams[0]);
	}

	void RenderPass_ShadowMap::sendProjectionInfo2Shader(Shader* shader, Camera* cam)
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

	void RenderPass_ShadowMap::renderPass(Camera* cam) {

		for (auto entity : scene->entities) {

			if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
				continue;

			glm::mat4 model = entity->transform.globalSpace();

			shadowmapShader->setMat4("model", model);

			sendProjectionInfo2Shader(shadowmapShader, cam);
			entity->meshRenderer.m_material->sendMaterial2Shader();

			entity->meshRenderer.Render();
		}
	}

}
