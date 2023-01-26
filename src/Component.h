#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_opengl3.h"

#include "logger.h"
#include <string>

#include "IEditorGUISelectable.h"

namespace Engine
{
	class Component : public IEditorGUISelectable
	{
	public:
		//Methods-------------------------------------

		virtual void drawEditorGUI_Properties() {
			log_error("ERROR::COMPONENT::BASE_CLASS_VIRTUAL_METHOD_CALLED: drawEditorGUI_Properties\n");
		};

		//Variables-----------------------------------

		std::string name;

	private:
		//Variables-----------------------------------


		//Methods-------------------------------------
	};
}

#endif
