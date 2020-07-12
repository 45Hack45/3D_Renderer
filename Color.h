#pragma once
#include "include/glm/glm.hpp"

struct Color {
	Color(float r, float g, float b, float a) {
		color_vect.r = r;
		color_vect.g = g;
		color_vect.b = b;
		color_vect.a = a;
	}
	Color(float r, float g, float b) {
		color_vect.r = r;
		color_vect.g = g;
		color_vect.b = b;
		color_vect.a = 1;
	}
	Color(glm::vec4 color) {
		this->color_vect = color;
	}
	Color(glm::vec3 color) {
		this->color_vect = glm::vec4(color.x, color.y, color.z, 1.0f);
	}
	Color() {
		color_vect.r = 0;
		color_vect.g = 0;
		color_vect.b = 0;
		color_vect.a = 0;
	}

	glm::vec4 getColorVec() const {
		return color_vect;
	}
	void setColor(float r, float g, float b, float a) {
		color_vect.r = r;
		color_vect.g = g;
		color_vect.b = b;
		color_vect.a = a;
	}

	Color operator*(float b) {
		Color c = (Color(glm::min(color_vect.r * b, 254.f), glm::min(color_vect.g * b, 254.f), glm::min(color_vect.b * b, 254.f), color_vect.a));
		return c;
	}
	Color operator+(Color b) {
		Color c = (Color(glm::min(color_vect.r + b.color_vect.r, 254.f), glm::min(color_vect.g + b.color_vect.g, 254.f), glm::min(color_vect.b + b.color_vect.b, 254.f), glm::min(color_vect.a + +b.color_vect.a, 254.f)));
		return c;
	}

	Color operator/(float a) { 
		return glm::vec4(color_vect) / a;
	}

	glm::vec4 color_vect;

	const static Color Black;
	const static Color Grey;
	const static Color White;
	const static Color Red;
	const static Color Green;
	const static Color Blue;
};

