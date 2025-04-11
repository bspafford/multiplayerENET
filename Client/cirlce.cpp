#include "circle.h"
#include "shaderClass.h"

circle::circle(float radius) {
	this->radius = radius;
	num_segments = round(radius);
	std::vector<float> vertices;

	// Center point
	//vertices.push_back(0.0f);
	//vertices.push_back(0.0f);

	// Perimeter points
	for (long i = 0; i < num_segments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		vertices.push_back(x);
		vertices.push_back(y);
	}

	// Generate and bind VAO & VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void circle::draw(Shader* shader, bool isWorld) {
	glUniform2f(glGetUniformLocation(shader->ID, "pos"), loc.x, loc.y);
	glUniform3f(glGetUniformLocation(shader->ID, "color"), r, g, b);
	glUniform1f(glGetUniformLocation(shader->ID, "radius"), radius);
	glUniform1i(glGetUniformLocation(shader->ID, "isWorld"), isWorld);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments);
	//glLineWidth(3.f);
	//glDrawArrays(GL_LINE_LOOP, 0, num_segments);
}

vector circle::getLoc() {
	return loc;
}

void circle::setLoc(vector loc) {
	this->loc = loc;
}

void circle::setColor(GLuint r, GLuint g, GLuint b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

float circle::getRadius() {
	return radius;
}

vector circle::getVelocity() {
	return velocity;
}

void circle::setVelocity(vector velocity) {
	this->velocity = velocity;
}