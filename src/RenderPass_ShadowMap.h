#pragma once

#include "Renderer.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "ShaderManager.hpp"


namespace Engine
{
	class RenderPass_ShadowMap: public IRenderPass
	{
	public:

		#define maxCascadeShadowMaps 4
		float shadowMap_Resolution = 1;
		int shadowMap_nCascades = 4;

		RenderPass_ShadowMap() {
			shadowmapShader = ShaderManager::Instance()->getShader("ShadowMapShader");

			shadowMap_nCascades = std::clamp(shadowMap_nCascades, 1, maxCascadeShadowMaps);

			shadow_fbo = new FrameBuffer(shadowMap_Resolution * 1024 * shadowMap_nCascades, shadowMap_Resolution * 1024, FrameBuffer::FrameType_Depth | FrameBuffer::FrameType_Color, 0, GL_RGBA16F);

			//Init aux cameras
			auxCams = new Camera[4];

			auxCams[0].ortoFrustrum_Horizontal = 32;
			auxCams[0].ortoFrustrum_Vertical = 32;
			auxCams[0].isOrtographic = true;

			auxCams[1].ortoFrustrum_Horizontal = 64;
			auxCams[1].ortoFrustrum_Vertical = 64;
			auxCams[1].isOrtographic = true;

			auxCams[2].ortoFrustrum_Horizontal = 250;
			auxCams[2].ortoFrustrum_Vertical = 250;
			auxCams[2].isOrtographic = true;

			auxCams[3].ortoFrustrum_Horizontal = 500;
			auxCams[3].ortoFrustrum_Vertical = 500;
			auxCams[3].isOrtographic = true;
		}

		//Draw.
		//Optional input framebuffer
		//Optional output framebuffer
		const FrameBuffer* RenderPass(const FrameBuffer* in_fbo);

		//Returns render pass render order. Lower value are rendered first
		unsigned int getRenderOrder() const { return IRenderPass::RenderPassOrder_FullScreen; }

		void RenderPassDebugGUI();

		const FrameBuffer* getFBO()const { return shadow_fbo; }

		void setScene(Scene* Scene) { scene = Scene; }
		void setMainCamera(Camera* camera) { cam = camera; }

		Camera* getAuxShadowmapCameras() { return auxCams; }

	private:
		Camera* cam = nullptr;//Main camera
		Camera* auxCams;//Shadowmap Aux Cameras

		Scene* scene = nullptr;
		Shader* shadowmapShader = nullptr;
		FrameBuffer* shadow_fbo = nullptr;

		float auxCamDistance = 500.f;
		float camDirOffsetFactor = 16.f * 0;

		void RenderCascadeShadowmaps(Shader* shader, glm::vec3 lightPos);
		void renderPass(Camera* cam);
		void sendProjectionInfo2Shader(Shader* shader, Camera* cam);
	
	};
}

