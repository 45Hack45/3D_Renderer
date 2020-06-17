#pragma once
#include <string>
#include "include/glm/glm.hpp"

#include "logger.h"

#include "Color.h"

const std::string lightSrcType_NONE = "NONE";
const std::string lightSrcType_Directional = "DirectionalLight";
const std::string lightSrcType_Spot = "SpotLight";
const std::string lightSrcType_Focus = "FocusLight";

enum LightSrcType
{
	NONE = 0,
	DIRECTIONAL = 1
};

class LightSource
{
public:

	LightSource(Color color, float intensity) {
		m_color = color;
		m_intensity = intensity;
	}

	void setColor(Color color) {
		m_color = color;
	}
	Color getColor() {
		return m_color;
	}

	//p = position from where we evaluate the direction
	virtual glm::vec3 getDirection(glm::vec3 p) {
		log_message(log_level_e::LOG_DEBUG, "Trying to use base class function: getDirection from LightSource\n");
		return glm::vec3(0);
	};
	//p = position from where we evaluate the intensity
	virtual float getIntensity(glm::vec3 p) {
		log_message(log_level_e::LOG_DEBUG, "Trying to use base class function: getIntensity from LightSource\n");
		return -1;
	};
	//p = position from where we evaluate the color
	virtual Color getColor(glm::vec3 p) {
		log_message(log_level_e::LOG_DEBUG, "Trying to use base class function: getIntensity from LightSource\n");
		return Color();
	};
	virtual LightSrcType getLightSrcType()const {
		log_message(log_level_e::LOG_DEBUG, "Trying to use base class function: getLightSrcType from LightSource\n");
		return LightSrcType::NONE;
	}

protected:

	Color m_color;
	float m_intensity;
};

class LightSource_Directional : public LightSource {
public:

	LightSource_Directional(Color color, float intensity, glm::vec3 direction) : LightSource(color, intensity), m_dir(direction) {};

	glm::vec3 getDirection(glm::vec3 p) { return m_dir; }
	float getIntensity(glm::vec3 p) { return m_intensity; }
	Color getColor(glm::vec3 p) { return m_color; }

	void setDirection(const glm::vec3& direction) { m_dir = direction; }

	LightSrcType getLightSrcType()const {
		return LightSrcType::DIRECTIONAL;
	}
private:
	glm::vec3 m_dir;
};