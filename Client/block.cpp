#include "block.h"

#include "img.h"
#include "shaderClass.h"
#include "main.h"
#include "timer.h"
#include "animation.h"
#include "character.h"

block::block(int blockId, vector loc) {
	this->loc = loc;

	//if (!blockSheet)
	//	blockSheet = new img("blockSheet.png", { 0, 0 }, true);

	blockImg = new img(std::to_string(blockId) + ".png", loc, true);

	breakTimer = new timer();
	breakTimer->addCallback(this, &block::breakBlock);
}

void block::draw(Shader* shaderProgram) {
	if (blockId == -1)
		return;

	blockImg->draw(shaderProgram);

	/*
	Rect rect = { 16, 0, 16, 16 };
	blockSheet->setSourceRect(&rect);
	blockSheet->setLoc(loc);
	blockSheet->draw(shaderProgram);
	*/

	bool mouseOver = isMouseOver();
	if (mouseOver && Main::getMouseInput(GLFW_MOUSE_BUTTON_LEFT)) {
		breakingBlock();
		Main::breakAnim->draw(shaderProgram);
	} else if (isBreaking && (!mouseOver || !Main::getMouseInput(GLFW_MOUSE_BUTTON_LEFT)))
		cancelBlockBreak();
}

void block::breakingBlock() {
	/*
	if (!isBreaking && Main::selectedBlock == this) {
		isBreaking = true;
		breakTimer->start(1);
		Main::breakAnim->setLoc(Main::selectedBlock->getLoc() * -Main::pixelSize);
		Main::breakAnim->start();
	}
	*/
}

void block::cancelBlockBreak() {
	Main::breakAnim->stop();
	breakTimer->stop();
	isBreaking = false;
}

void block::breakBlock() {
	blockId = -1;
}

bool block::isMouseOver() {
	// if close enough
	float dist = math::distance(blockImg->loc, Main::character->getLoc() / Main::pixelSize);
	return dist < 66.f && blockImg->mouseOver();
}

vector block::getLoc() {
	return loc;
}