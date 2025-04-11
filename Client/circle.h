#pragma once

#include <glad/glad.h>

#include "math.h"

class Shader;

class circle {
public:
	circle(float radius);
	virtual void draw(Shader* shader, bool isWorld = false);
	virtual vector getLoc();
	virtual void setLoc(vector loc);
	virtual void setColor(GLuint r, GLuint g, GLuint b);
	float getRadius();
	vector getVelocity();
	void setVelocity(vector velocity);

private:
	long num_segments = 100;
	GLuint VAO, VBO;
	GLuint r = 255, g = 255, b = 255;
	float radius;
	
protected:
	vector loc;
	vector velocity;
};