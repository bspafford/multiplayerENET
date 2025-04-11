#pragma once

#include "math.h"
#include <glm/glm.hpp>

class img;
class Shader;
class timer;

class block {
public:
	block(int blockId, vector loc);
	void draw(Shader* shaderProgram);
	bool isMouseOver();
	vector getLoc();
	void breakingBlock();
	void cancelBlockBreak();
	void breakBlock();

private:
	static inline img* blockSheet;

	img* blockImg;
	vector loc;
	int blockId;

	timer* breakTimer; // prolly shouldn't have a timer for every block, make this static, then have a static selectedBlock variable
	bool isBreaking = false;
	bool breakingPrev = false;
};