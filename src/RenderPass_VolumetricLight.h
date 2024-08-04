#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"

namespace Engine
{
	class RenderPass_VolumetricLight: IRenderPass
	{
	public:
		//Draw.
		//Optional input framebuffer
		//Optional output framebuffer
		const FrameBuffer* RenderPass(const FrameBuffer* in_fbo);

		//Returns render pass render order. Lower value are rendered first
		unsigned int getRenderOrder() const { return IRenderPass::RenderPassOrder_FullScreen; }

	private:
		Camera* cam;
		Scene* scene;
	};
}

