#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <chrono>

#pragma once

class Shader;
class img;
class Font;
class TextRenderer;
#include "math.h"

class item {
public:
	item(int id, int quantity);
	void draw(Shader* shaderProgram);
	void setItem(int itemId);
	void setLoc(vector loc);
	int getId();
	int getQuantity();
	void remove1();
	void add1();

private:
	img* itemImg;

	int id;
	int quantity;

	Font* font;
	TextRenderer* textRenderer;
};