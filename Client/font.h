#pragma once

#include "stb_truetype.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "math.h"

class Shader;

class Font {
public:
	Font(std::string text, std::string fontPath, float fontSize);
	void draw();
	void setLoc(vector loc);
	vector getSize();
	void setText(std::string text);

private:
	void renderText(GLuint shaderProgram, GLuint VAO, GLuint VBO, GLuint texture, stbtt_bakedchar* cdata, const char* text, float x, float y, float scale, glm::vec3 color);
	GLuint loadFont(const char* fontPath, float fontSize, stbtt_bakedchar* cdata);

	vector loc;
	vector size;
	std::string text;

	GLuint fontTexture;
	GLuint VAO, VBO;
	stbtt_bakedchar cdata[96];
};