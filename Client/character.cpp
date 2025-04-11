#include "character.h"
#include "main.h"
#include "camera.h"

Acharacter::Acharacter(float radius) : circle(radius) {

}

void Acharacter::Update(float deltaTime) {
	move(deltaTime);

	if (Main::isGravityEnabled) {
		//velocity.y += .981 * deltaTime;

		vector circleLoc = Main::circle1->getLoc();
		vector normal = math::normalize(circleLoc - getLoc()) * deltaTime * .981 * 2000.f;
		velocity = velocity + normal;

		setLoc(loc + velocity * deltaTime);
	}

	if (jumpOnCooldown) {
		jumpTime += deltaTime;
		//std::cout << "jumptime: " << jumpTime << ", jumpTimer: " << jumpTimer << ", cooldown: " << jumpOnCooldown << std::endl;
		if (jumpTime >= jumpTimer) {
			jumpOnCooldown = false;
			jumpTime = 0;
		}
	}

	vector drag = vector{ 100, 100 } * deltaTime;
	if (velocity.x < 0)
		drag.x *= -1;
	if (velocity.y < 0)
		drag.y *= -1;
	velocity = velocity - drag;
}

void Acharacter::draw(Shader* shader, bool isWorld) {
	__super::draw(shader);
}

void Acharacter::move(float deltaTime) {
	isTouchingGround();

	vector circleLoc = Main::circle1->getLoc();
	vector normal = math::normalize(circleLoc - getLoc());

	if (Main::KEYS[GLFW_KEY_A]) {
		// temp should use velocity
		setLoc(loc - vector{-normal.y, normal.x} * deltaTime * speed);
	}

	if (Main::KEYS[GLFW_KEY_D]) {
		setLoc(loc + vector{ -normal.y, normal.x } * deltaTime * speed);
	}

	if (Main::KEYS[GLFW_KEY_SPACE] && isTouchingGround() && !jumpOnCooldown) {
		std::cout << "jumping!" << std::endl;
		//setVelocity(velocity - normal * .25f);
		setVelocity(velocity - normal * 500.f);
		jumpOnCooldown = true;
	}

	Main::camera->setLoc(loc);
}

bool Acharacter::isTouchingGround() {
	//return math::distance(loc, Main::circle1->getLoc()) <= getRadius() + Main::circle1->getRadius() + .1f;

	long long index = Main::getBlockIndexFromWorldPos(getLoc());
	if (index == -1)
		return false;

	// get the block below the character
	vector charBlockPos = Main::getCharacterBlockPos();
	float yLoc = (Main::getChunkPos().y - charBlockPos.y) / Main::blockSize;
	long long blockBelowChar = -1;
	for (int i = 0; i < Main::chunkSize.y - yLoc; i++) {
		long long num = index + Main::chunkSize.x * i;
		if (Main::blocks[num].textureIndex != 0) {
			blockBelowChar = num;
			break;
		}
	}

	float charLoc = getLoc().y - getRadius();
	float blockLoc = Main::blocks[blockBelowChar].position.y + Main::blockSize;
	float dist = blockLoc - charLoc;

	if (dist < .1)
		return true;
	return false;
}