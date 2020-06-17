#pragma once
#include "include/glm/glm.hpp"

struct Color {
	Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	Color(glm::vec4 color) {
		this->color = color;
	}
	Color() {
		color.r = 0;
		color.g = 0;
		color.b = 0;
		color.a = 0;
	}

	glm::vec4 getColor() const {
		return color;
	}
	void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	Color operator*(float b) {
		Color c = (Color(glm::min(color.r * b, 254.f), glm::min(color.g * b, 254.f), glm::min(color.b * b, 254.f), color.a));
		return c;
	}
	Color operator+(Color b) {
		Color c = (Color(glm::min(color.r + b.color.r, 254.f), glm::min(color.g + b.color.g, 254.f), glm::min(color.b + b.color.b, 254.f), glm::min(color.a + +b.color.a, 254.f)));
		return c;
	}

	Color operator/(float a) { 
		return glm::vec4(color) / a;
	}
private:
	glm::vec4 color;
};