#include "math.h"
#include "main.h"
#include "camera.h"
#include <glm/glm.hpp>

// returns the dot product of two 2d vectors
float math::dot(vector a, vector b) {
	return a.x * b.x + a.y * b.y;
}

// normalizes the vector
vector math::normalize(vector a) {
	float invLen = Q_rsqrt(a.x * a.x + a.y * a.y);

	if (a.x == 0 && a.y == 0)
		return { 0, 0 };

	return { a.x * invLen, a.y * invLen };

}

float math::length(vector value) {
	return Q_sqrt(value.x * value.x + value.y * value.y);
}


// returns the distance between 2 points
float math::distance(float x1, float y1, float x2, float y2) {
	return Q_sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));

}

float math::distance(vector a, vector b) {
	return Q_sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}

float math::min(float a, float b) {
	if (a < b) return a;
	return b;
}

float math::max(float a, float b) {
	if (a > b) return a;
	return b;
}

float math::lerp(float min, float max, float val) {
	float dif = max - min;
	return val * dif + min;
}

float math::randRange(float min, float max) {
	return (float)rand() / RAND_MAX * (max - min) + min;
}

float math::clamp(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

float math::Q_sqrt(float number) {
	return Q_rsqrt(number) * number;
}

float math::Q_rsqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;

	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y)); // adds extra accuracy

	return y;
}

vector math::screenToWorld(vector screenPos) {
	glm::vec4 ndc = glm::vec4(
		(2.0f * Main::mousePos.x) / Main::screenSize.x - 1.0f,
		1.0f - (2.0f * Main::mousePos.y) / Main::screenSize.y,
		0, 1.0f);

	// Inverse transformation
	glm::mat4 invVP = glm::inverse(Main::camera->camMatrix);
	glm::vec4 worldPos = invVP * ndc;

	return vector{ worldPos.x, worldPos.y } / worldPos.w;
}