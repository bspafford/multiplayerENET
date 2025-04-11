#pragma once

#include "circle.h"
#include "math.h"

class Acharacter : public circle {
public:
	Acharacter(float radius);
	void Update(float deltaTime);
	void draw(Shader* shader, bool isWorld = false) override;
	void move(float deltaTime);

	bool isTouchingGround();

private:
	float speed = 250.f;

	bool jumpOnCooldown = false;
	float jumpTimer = .5f, jumpTime = 0;
};
