#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"

namespace Engine
{
	class RenderPass_Transparent: public IRenderPass
	{
	public:
		RenderPass_Transparent() {
			Renderer::Instance()->addRenderPass(this);

		}

		//Draw.
		//Optional input framebuffer
		//Optional output framebuffer
		const FrameBuffer* RenderPass(const FrameBuffer* in_fbo);

		//Returns render pass render order. Lower value are rendered first
		unsigned int getRenderOrder() const { return IRenderPass::RenderPassOrder_Transparent; }

		void RenderPassDebugGUI();

		void setMainCamera(Camera* camera) { cam = camera; }
		void setShadowmapCameras(Camera* cameras) { auxCam_shadowmap = cameras; }
		void setScene(Scene* Scene) { m_scene = Scene; }

	private:
		Camera* cam;
		Camera* auxCam_shadowmap;
		Scene* m_scene;

		float shadowBias = 16;
		float shadowBiasMax = 8;

		float ambientLight = 0.05f;

		void sendProjectionInfo2Shader(Shader* shader, Camera* cam);
		void sendCascadeShadowMapInfo2Shader(Shader* shader, const FrameBuffer* shadow_fbo);
		void sendLightInfo2Shader(Shader* shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight);

	};
}

