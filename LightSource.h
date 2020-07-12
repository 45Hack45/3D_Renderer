#pragma once
#include <string>
#include "include/glm/glm.hpp"

#include "logger.h"

#include "Color.h"

//const std::string lightsrctype_none = "none";
//const std::string lightsrctype_directional = "directionallight";
//const std::string lightsrctype_spot = "spotlight";
//const std::string lightsrctype_focus = "focuslight";

enum LightSrcType
{
	NONE = 0,
	DIRECTIONAL = 1,
	SPOT = 2,
	POINT = 3
};

class LightSource
{
public:

	LightSource() {
		m_color = Color(255,255,255);
		m_intensity = 1.f;
	}
	LightSource(Color color, float intensity) {
		m_color = color;
		m_intensity = intensity;
	}

	void setColor(const Color& color) {
		m_color = color;
	}
	Color getColor()const {
		return m_color;
	}

	float getIntensity()const {
		return m_intensity;
	};
	void setIntensity(float intensity) {
		m_intensity = intensity;
	};

	virtual LightSrcType getLightSrcType()const {
		log_message(log_level_e::LOG_DEBUG, "ERROR::LIGHTSOURCE::Trying to use base class function: getLightSrcType from LightSource\n");
		return LightSrcType::NONE;
	}

	Color m_color;
	float m_intensity;
};

class LightSource_Directional : public LightSource {
public:

	LightSource_Directional(Color color, float intensity, glm::vec3 direction) : LightSource(color, intensity), m_dir(direction) {};

	glm::vec3 getDirection(glm::vec3 p) { return m_dir; }
	void setDirection(const glm::vec3& direction) { m_dir = direction; }

	LightSrcType getLightSrcType()const {
		return LightSrcType::DIRECTIONAL;
	}

	glm::vec3 m_dir;
};

class LightSource_Spot : public LightSource {
public:

	LightSource_Spot(Color color, float intensity, glm::vec3 position, glm::vec3 direction, float angle, float outerAngle) : LightSource(color, intensity) {
		m_pos = position;
		m_dir = direction;
		m_angle = angle;
		m_linear = 0.0014f;
		m_quadratic = 0.000007f;
	};

	LightSource_Spot(Color color, float intensity, glm::vec3 position, glm::vec3 direction, float angle, float outerAngle, float linear, float quadratic) : LightSource(color, intensity) {
		m_pos = position;
		m_dir = direction;
		m_angle = angle;
		m_outerAngle = outerAngle;
		m_linear = linear;
		m_quadratic = quadratic;
	};

	glm::vec3 getDirection(glm::vec3 p) { return m_dir; }
	void setDirection(const glm::vec3& direction) { m_dir = direction; }

	glm::vec3 getPosition(glm::vec3 p) { return m_pos; }
	void setPosition(const glm::vec3& position) { m_pos = position; }

	LightSrcType getLightSrcType()const {
		return LightSrcType::SPOT;
	}

	glm::vec3 m_dir;
	glm::vec3 m_pos;
	float m_angle;
	float m_outerAngle;
	float m_linear, m_quadratic;
};

class LightSource_Point : public LightSource {
public:
	LightSource_Point(Color color, float intensity, glm::vec3 position) : LightSource(color, intensity) {
		m_pos = position;
		m_linear = 0.0014f;
		m_quadratic = 0.000007f;
	};
	LightSource_Point(Color color, float intensity, glm::vec3 position, float linear, float quadratic) : LightSource(color, intensity){
		m_pos = position;
		m_linear = linear;
		m_quadratic = quadratic;
	};

	glm::vec3 getPosition()const { return m_pos; }
	void setPosition(const glm::vec3& position) { m_pos = position; }

	float getLinear()const { return m_linear; }
	void setLinear(float value) { m_linear = value; }

	float getQuadratic()const { return m_quadratic; }
	void setQuadratic(float value) { m_quadratic = value; }

	void setColor(const Color& color) {
		m_color = color;
	}
	Color getColor()const {
		return m_color;
	}

	LightSrcType getLightSrcType()const {
		return LightSrcType::POINT;
	}

	glm::vec3 m_pos;
	float m_linear, m_quadratic;
};


