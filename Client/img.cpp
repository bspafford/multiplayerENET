#include "img.h"
#include "main.h"
#include "camera.h"

img::img(std::string image, vector loc, bool useWorldPos) {
	GLenum texType = GL_TEXTURE_2D;
	GLenum slot = GL_TEXTURE0;
	GLenum format = GL_RGBA;
	GLenum pixelType = GL_UNSIGNED_BYTE;

	this->useWorldPos = useWorldPos;

	this->loc = loc;
	int width, height, nChannels;
	unsigned char* bytes = stbi_load(std::string("images/" + image).c_str(), &width, &height, &nChannels, NULL);

	ogW = width;
	ogH = height;
	w = width;
	h = height;

	// should make a function for this, to calculate, so i can call it from the animation class
	vector scale = vector{ w * Main::pixelSize, h * Main::pixelSize};
	vector scaledLoc = loc;
	if (useWorldPos)
		scaledLoc = loc * Main::pixelSize;

	float positions[] = {
		// Positions // Texture Coords
		scale.x + scaledLoc.x, scaledLoc.y, 1.f, 1.f,           // Bottom-right
		scale.x + scaledLoc.x, scale.y + scaledLoc.y, 1.f, 0.f, // Top-right
		scaledLoc.x, scale.y + scaledLoc.y, 0.f, 0.f,           // Top-left
		scaledLoc.x, scaledLoc.y, 0.f, 1.f                      // Bottom-left
	};

	std::vector<GLuint> indices = {
		0, 1, 3, // First triangle
		3, 1, 2  // Second triangle

		// 0, 1, 2, // First triangle
		// 0, 2, 3  // Second triangle
	};

	currVAO = new VAO();
	currVAO->Bind();

	//currVBO = new VBO(vertices);
	glGenBuffers(1, &VBOId);
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	currEBO = new EBO(indices);


	//currVAO->LinkVBO(*currVBO, 0);
	//currVAO->LinkAttrib(*currVBO, 0, 1, NULL, 3, 0);
	//currVBO->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//currVBO->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Assigns the type of the texture ot the texture object
	type = texType;

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);

	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(texType, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Determine the pixel format of the surface.
	//GLint nOfColors = imgSurface->format->BytesPerPixel;
	GLenum textureFormat;
	if (nChannels == 4) { // Image has an alpha channel
		textureFormat = GL_RGBA;
	} else if (nChannels == 3) { // No alpha channel
		textureFormat = GL_RGB;
	} else {
		std::cerr << "Warning: the image is not truecolor; this may cause issues."
			<< std::endl;
	}

	// Assigns the image to the OpenGL Texture object
	//glTexImage2D(texType, 0, nChannels, w, h, 0, textureFormat, pixelType, bytes);
	glTexImage2D(texType, 0, textureFormat, width, height, 0, textureFormat, pixelType, bytes);

	// Generates MipMaps
	glGenerateMipmap(texType);

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);
}

void img::draw(Shader* shaderProgram) {
	shaderProgram->Activate();
	glActiveTexture(GL_TEXTURE0);
	Bind();
	currVAO->Bind();


	glUniform1i(glGetUniformLocation(shaderProgram->ID, "useWorldPos"), useWorldPos);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	Unbind();
	currVAO->Unbind();
}

void img::setSourceRect(Rect* source) {
	this->source = source;

	updatePositionsList();
}

void img::setLoc(vector loc) {
	this->loc = loc;

	updatePositionsList();
}

void img::updatePositionsList() {
	currVAO->Bind();
	//currVBO->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);

	// source : w, h
	float x1, x2, y1, y2;
	if (source) {
		x1 = source->x / ogW;
		x2 = (source->x + source->w) / ogW;
		y1 = source->y / ogH;
		y2 = (source->y + source->h) / ogH;
	} else {
		x1 = 0;
		x2 = 1;
		y1 = 0;
		y2 = 1;
	}

	vector scale = vector{ w , h } * Main::pixelSize;
	vector scaledLoc = loc * -1;
	if (useWorldPos)
		scaledLoc = loc * Main::pixelSize;
	float positions[] = {
		// Positions						// Texture Coords
		scale.x + scaledLoc.x, scaledLoc.y,				x2, y2,
		scale.x + scaledLoc.x, scale.y + scaledLoc.y,	x2, y1,
		scaledLoc.x, scale.y + scaledLoc.y,	 			x1, y1,
		scaledLoc.x, scaledLoc.y,						x1, y2
	};

	// updates the tex coords
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ptr) {
		memcpy(ptr, positions, sizeof(positions)); // Copy updated vertex data
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

void img::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void img::Bind() {
	glBindTexture(type, ID);
}

void img::Unbind() {
	glBindTexture(type, 0);
}

void img::Delete() {
	glDeleteTextures(1, &ID);
}

bool img::mouseOver() {
	vector worldPos = math::screenToWorld(Main::mousePos);
	
	vector scale = vector{ w , h } * Main::pixelSize;
	vector scaledLoc = loc * -1;
	if (useWorldPos)
		scaledLoc = loc * Main::pixelSize;

	bool inX = worldPos.x >= scaledLoc.x && worldPos.x <= scaledLoc.x + scale.x;
	bool inY = worldPos.y >= scaledLoc.y && worldPos.y <= scaledLoc.y + scale.y;
	return inX && inY;
}

vector img::getSize() {
	return vector{ w, h } * Main::pixelSize;
}

void img::setSize(vector size) {
	w = size.x;
	h = size.y;
	updatePositionsList();
}