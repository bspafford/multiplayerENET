#pragma once

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "math.h"

class Shader;

class Camera {
public:
	Camera();
	void Update(Shader* shader, float deltaTime);
	void setLoc(vector loc);
	void move(float deltaTime);

	glm::mat4 camMatrix;
private:
	vector loc;
};