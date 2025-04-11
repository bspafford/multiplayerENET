#include "camera.h"
#include "main.h"
#include "shaderClass.h"
#include "circle.h"
#include "character.h"

Camera::Camera() {
	
}

void Camera::Update(Shader* shader, float deltaTime) {
	if (Main::cameraFollow) {
		vector circleLoc = Main::circle1->getLoc();
		vector normal = math::normalize(circleLoc - Main::character->getLoc()) * -1.f * deltaTime * .981;

		glm::vec3 up(normal.x, normal.y, 0.f);
		glm::mat4 view(1.f);
		glm::vec3 orientation = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));

		float left = -Main::screenSize.x / 2.f, right = Main::screenSize.x / 2.f, top = Main::screenSize.y / 2.f, bottom = -Main::screenSize.y / 2.f;
		//float left = 0, right = 8, top = 6, bottom = 0;
		glm::mat4 ortho = glm::ortho(left, right, bottom, top);

		glm::vec3 pos(loc.x, loc.y, 0);
		view = glm::lookAt(pos, pos + orientation, up);

		camMatrix = ortho * view;

		// Exports camera matrix
		//glUniform2f(glGetUniformLocation(shader->ID, "camPos"), loc.x, loc.y);
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camMatrix));

		move(deltaTime);
	} else {
		vector circleLoc = Main::circle1->getLoc();
		vector normal = math::normalize(circleLoc - Main::character->getLoc()) * 1.f * deltaTime * .981;

		glm::vec3 up(0.f, 1.f, 0.f);
		glm::mat4 view(1.f);
		glm::vec3 orientation = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));

		float min = math::min(Main::screenSize.x, Main::screenSize.y);
		float multiplier = 2.5f * Main::circle1->getRadius() / min;

		float left = -Main::screenSize.x / 2.f * multiplier, right = Main::screenSize.x / 2.f * multiplier, top = Main::screenSize.y / 2.f * multiplier, bottom = -Main::screenSize.y / 2.f * multiplier;
		glm::mat4 ortho = glm::ortho(left, right, bottom, top);

		glm::vec3 pos(circleLoc.x, circleLoc.y, 1.f);
		view = glm::lookAt(pos, pos + orientation, up);

		glm::mat4 cameraMatrix = ortho * view;
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	}
}

void Camera::move(float deltaTime) {
	/*
	if (Main::KEYS[GLFW_KEY_LEFT]) {
		loc = loc + vector{-100.f * deltaTime, 0};
	}

	if (Main::KEYS[GLFW_KEY_RIGHT]) {
		loc = loc + vector{ 100.f * deltaTime, 0 };
	}
	*/
}

void Camera::setLoc(vector loc) {
	this->loc = loc;
}