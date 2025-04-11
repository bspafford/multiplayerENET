#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include <stb/stb_image.h>
#include <vector>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include"shaderClass.h"

#include "math.h"

class img {
public:
	GLuint ID;
	GLenum type;
	img(std::string image, vector loc, bool useWorldPos);

	void draw(Shader* shaderProgram);
	void setSourceRect(Rect* rect);
	void setLoc(vector loc);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
	bool mouseOver();
	vector getSize();
	void setSize(vector size);

	VAO* currVAO;
	//VBO* currVBO;
	// temp
	GLuint VBOId;
	EBO* currEBO;

	bool useWorldPos;

	vector loc;
	float w = 0, h = 0;
	// scale of image without a source rect
	float ogW = 0, ogH = 0;
	Rect* source;

private:
	void updatePositionsList();
};
#endif