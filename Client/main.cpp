#include "main.h"
#include "shaderClass.h"
#include "img.h"
#include "circle.h"
#include "character.h"
#include "camera.h"
#include "block.h"
#include "animation.h"
#include "timer.h"
#include "hotbar.h"
#include "item.h"
#include "client.h"

unsigned char* Main::extractTile(unsigned char* atlas, int atlasWidth, int atlasHeight,
	int tileX, int tileY, int tileWidth, int tileHeight, int channels) {

	// Check if the tile dimensions fit within the atlas boundaries
	if (tileX + tileWidth > atlasWidth || tileY + tileHeight > atlasHeight) {
		std::cerr << "Tile extraction out of bounds!" << std::endl;
		return nullptr;  // Return nullptr to signal an error
	}

	// Allocate memory for the extracted tile
	unsigned char* tile = new unsigned char[tileWidth * tileHeight * channels];

	// Loop through the tile's pixels and copy data from the atlas
	for (int y = 0; y < tileHeight; y++) {
		for (int x = 0; x < tileWidth; x++) {
			// Compute the position in the atlas and the tile
			int atlasIndex = ((tileY + y) * atlasWidth + (tileX + x)) * channels;
			int tileIndex = (y * tileWidth + x) * channels;

			// Copy each color channel from the atlas to the tile
			for (int c = 0; c < channels; c++) {
				tile[tileIndex + c] = atlas[atlasIndex + c];
			}
		}
	}

	return tile;  // Return the extracted tile
}

int main() {
	Main* _main = new Main();
	_main->createWindow();
	return 0;
}

int Main::createWindow() {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cout << "bad glfwInit" << std::endl;
		return -1;
	}

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); // Enable 4x multisampling

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	window = glfwCreateWindow(screenSize.x, screenSize.y, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	if (!gladLoadGL(glfwGetProcAddress)) {
		return -1;
	}
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, screenSize.x, screenSize.y);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetFramebufferSizeCallback(window, windowSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader* shaderProgram = new Shader("default.vert", "default.frag");
	Shader* textureShader = new Shader("textureShader.vert", "textureShader.frag");
	Shader* instanceShader = new Shader("instanceShader.vert", "instanceShader.frag");
	textShader = new Shader("text.vert", "text.frag");

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	shaderProgram->Activate();
	camera = new Camera();
	circle1 = new circle(3963.1f * 500.f);
	//circle1 = new circle(3963.1f * 100.f);
	//circle1 = new circle(200.f);
	character = new Acharacter(20.f);
	character->setLoc(circle1->getLoc() + vector{ 380, (circle1->getRadius() + character->getRadius() + 20) });
	circle* testCircle = new circle(10.f);

	// Start
	Start();

	outline = new img("outline.png", { 0, 0 }, true);
	std::unordered_map<std::string, animDataStruct> animData;
	animData.insert({ "anim", {{0, 0}, {9, 0}, .1, false} });
	breakAnim = new animation("breakAnim.png", 16, 16, animData, true, { 0, 0 });
	breakAnim->setAnimation("anim");

	framebuffer_size_callback(window, screenSize.x, screenSize.y);




	// Define the square (block) vertices
	blockSize = 16.f * pixelSize;
	float vertices[] = {
		0.0f,  0.0f,
		blockSize,  0.0f,
		blockSize,  blockSize,
		0.0f,  blockSize
	};

	// Block positions (instanced)
	int xNum = 16;
	int yNum = 320 + 64;
	for (int y = 0; y < yNum; y++) {
		for (int x = 0; x < xNum; x++) {
			//if (y > 0)
			int blockId = 0;
			if (y <= 255)
				blockId = 0;
			else if (y == 256)
				blockId = 2;
			else if (y <= 260)
				blockId = 3;
			else
				blockId = 1;
			blocks.push_back({ glm::vec2(x * blockSize, -y * blockSize - blockSize + circle1->getRadius() + 256.f * blockSize), float(blockId) }); // float((x + y) % 4) Cycle through 4 different textures
		}
	}

	// make tree 
	// logs
	blocks[4090].textureIndex = 17;
	blocks[4074].textureIndex = 17;
	blocks[4058].textureIndex = 17;
	blocks[4042].textureIndex = 17;
	// leaves
	blocks[4026].textureIndex = 18;
	blocks[4010].textureIndex = 18;
	blocks[4009].textureIndex = 18;
	blocks[4025].textureIndex = 18;
	blocks[4011].textureIndex = 18;
	blocks[4027].textureIndex = 18;
	blocks[4057].textureIndex = 18;
	blocks[4041].textureIndex = 18;
	blocks[4056].textureIndex = 18;
	blocks[4040].textureIndex = 18;
	blocks[4059].textureIndex = 18;
	blocks[4043].textureIndex = 18;
	blocks[4060].textureIndex = 18;
	blocks[4044].textureIndex = 18;

	instanceShader->Activate();
	// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	// Vertex Buffer for the square
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Generate Instance Buffer
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, blocks.size() * sizeof(BlockInstance), &blocks[0], GL_STATIC_DRAW);

	// Position Attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)0);
	glVertexAttribDivisor(1, 1);

	// Texture Index Attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)(sizeof(glm::vec2)));
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);



	GLuint textureAtlas;
	glGenTextures(1, &textureAtlas);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureAtlas);

	// Set parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	int width, height, nChannels;
	unsigned char* data = stbi_load("images/blockSheet.png", &width, &height, &nChannels, NULL); // Replace with actual loading function

	int blockNum = width / 16;

	std::cout << "width: " << width << ", " << height << ", " << nChannels << std::endl;

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, (nChannels == 4) ? GL_RGBA : GL_RGB, 16, 16, blockNum, 0, (nChannels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	// Load textures into layers
	for (int i = 0; i < blockNum; i++) {
		unsigned char* tileData = extractTile(data, width, height, i * 16, 0, 16, 16, nChannels);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 16, 16, 1, (nChannels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tileData);
		delete[] tileData;
	}
	stbi_image_free(data);

	// Bind the texture array before rendering
	instanceShader->Activate();
	glUniform1i(glGetUniformLocation(instanceShader->ID, "textureAtlas"), 0);











	auto lastTime = std::chrono::steady_clock::now();
	// Main while loop
	float temp = 0;
	while (!glfwWindowShouldClose(window)) {
		//Sleep(1.f / 60.f * 1000.f);
		auto currentTime = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		Update(deltaTime);

		temp += deltaTime;
		if (temp >= 1.f) {
			//std::cout << "fps: " << 1 / deltaTime << std::endl;
			temp = 0;
		}

		//float left = -screenSize.x / 2.f, right = screenSize.x / 2.f, top = -screenSize.y / 2.f, bottom = screenSize.y / 2.f;
		//glm::mat4 ortho = glm::ortho(left, right, bottom, top);
		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "projection"), 1, GL_FALSE, glm::value_ptr(ortho));
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.431f, 0.694f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram->Activate();
		// test collision
		// get radius add together
		// if closer than added radius then push back using the normal from their locs and move back the distance of overlap (total radius - ...)
		float totalRadius = character->getRadius() + circle1->getRadius();
		float distance = math::distance(circle1->getLoc(), character->getLoc());
		//std::cout << "distance: " << distance << std::endl;


		bool gravityEffectsWorld = false; // if you want the world to move from the gravity of the character
		if (isGravityEnabled) {
			if (gravityEffectsWorld) {
				vector normal = math::normalize(circle1->getLoc() - character->getLoc()) * deltaTime;
				vector pushCharacter = normal * circle1->getRadius() / 100.f;
				vector pushCircle = normal * character->getRadius() / 100.f;

				character->setVelocity(character->getVelocity() + pushCharacter);
				character->setLoc(character->getLoc() + character->getVelocity());

				circle1->setVelocity(circle1->getVelocity() - pushCircle);
				circle1->setLoc(circle1->getLoc() + circle1->getVelocity());
			}
		}

		testCollision();

		shaderProgram->Activate();
		character->Update(deltaTime);
		character->draw(shaderProgram);
		for (auto currChar : otherCharacters) {
			if (currChar.second)
				currChar.second->draw(shaderProgram);

		}

		//circle1->draw(shaderProgram, true);

		camera->Update(shaderProgram, deltaTime);
		float min = math::min(Main::screenSize.x, Main::screenSize.y);
		float multiplier = 1;// 2.5f * Main::circle1->getRadius() / min;
		float left = 0 * multiplier, right = Main::screenSize.x * multiplier, top = Main::screenSize.y * multiplier, bottom = 0 * multiplier;
		glm::mat4 ortho = glm::ortho(left, right, bottom, top);
		textShader->Activate();
		glUniformMatrix4fv(glGetUniformLocation(textureShader->ID, "screenProjMatrix"), 1, GL_FALSE, glm::value_ptr(ortho));


		textureShader->Activate();
		glUniformMatrix4fv(glGetUniformLocation(textureShader->ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera->camMatrix));
		glUniformMatrix4fv(glGetUniformLocation(textureShader->ID, "screenProjMatrix"), 1, GL_FALSE, glm::value_ptr(ortho));
		glUniform1i(glGetUniformLocation(textureShader->ID, "useWorldPos"), 1);


		selectedBlock = nullptr;
		vector mouseBlockPos = getMouseBlockPos();
		for (auto it = blocks.begin(); it != blocks.end(); ++it) {
			if (it->position == glm::vec2(mouseBlockPos.x, mouseBlockPos.y)) {
				selectedBlock = new blockStruct{ {it->position.x, it->position.y }, it->textureIndex, std::distance(blocks.begin(), it) };
				break;
			}
		}

		instanceShader->Activate();
		glUniformMatrix4fv(glGetUniformLocation(instanceShader->ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera->camMatrix));
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureAtlas);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, blocks.size());

		if (selectedBlock) {
			outline->setLoc(vector{ mouseBlockPos.x, mouseBlockPos.y } / pixelSize - 1);
			outline->draw(textureShader);

			if (getMouseInput(GLFW_MOUSE_BUTTON_LEFT)) {
				//breakBlock();
				breakingBlock();
				if (selectedBlock->blockId != 0)
					breakAnim->draw(textureShader);
			}
		}

		if (getMouseInput(GLFW_MOUSE_BUTTON_RIGHT) && getCharacterBlockPos() != getMouseBlockPos() && hotbar->getSelectedItem()->getId() != 0 && hotbar->getSelectedItem()->getQuantity() != 0) {
			long long index = getBlockIndexFromWorldPos(math::screenToWorld(mousePos));
			if (index != -1 && blocks[index].textureIndex == 0) {
				blocks[index].textureIndex = hotbar->items[hotbar->getSelectedBar()]->getId();
				client->changeBlockAtIndex(selectedBlock->posInChunk, hotbar->items[hotbar->getSelectedBar()]->getId());
				hotbar->items[hotbar->getSelectedBar()]->remove1();
				updateChunk();

			}
		}

		// if selectedBlock changes while mouse button down

		if (isBreaking && ((selectedBlock && selectedBlock->pos != prevSelectedBlock.pos) || !Main::getMouseInput(GLFW_MOUSE_BUTTON_LEFT)))
			cancelBlockBreak();

		if (selectedBlock)
			prevSelectedBlock = *selectedBlock;


		draw(textureShader);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, screenSize.x, screenSize.y, 0, 0, screenSize.x, screenSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glfwSwapBuffers(window);
		glfwPollEvents();


		// checks for errors
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL Error: " << err << std::endl;
		}
	}



	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void Main::Start() {
	breakTimer = new timer();
	breakTimer->addCallback(this, &Main::breakBlock);

	hotbar = new Uhotbar();

	client = new Client();
}

void Main::Update(float deltaTime) {
	timer::callUpdate(deltaTime);

	client->sendGameInfo();
}

void Main::draw(Shader* shaderProgram) {
	hotbar->draw(shaderProgram);
}

void Main::windowSizeCallback(GLFWwindow* window, int width, int height) {
	screenSize = { float(width), float(height) };
	glViewport(0, 0, width, height);

	framebuffer_size_callback(window, width, height);
}

void Main::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_B) {
			isGravityEnabled = !isGravityEnabled;
		}

		if (key == GLFW_KEY_R) {
			character->setLoc(circle1->getLoc() + vector{ 0, -(circle1->getRadius() + character->getRadius()) });
			character->setVelocity({ 0, 0 });

			circle1->setVelocity({ 0, 0 });
		}

		if (key == GLFW_KEY_E) {
			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);
			character->setLoc(vector{ (float)xPos, (float)yPos } - screenSize / 2.f);
		}

		if (key == GLFW_KEY_Q) {
			cameraFollow = !cameraFollow;
		}

		std::vector<int> hotbarList = std::vector<int>{ GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
		if (std::find(hotbarList.begin(), hotbarList.end(), key) != hotbarList.end()) {
			hotbar->setSelectedBar(key - 49);
		}

		//std::cout << "pressed key: " << key << std::endl;
		KEYS[key] = true;
	} else if (action == GLFW_RELEASE) {
		KEYS[key] = false;
	}
}

void Main::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	// Recreate the framebuffer
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &rbo);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Create a multisampled texture with new dimensions
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

	// Create a new multisampled renderbuffer for depth & stencil
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check framebuffer completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: Framebuffer is not complete after resizing!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Main::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
	mousePos = { float(xPos), float(yPos) };
}

void Main::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	int num = hotbar->getSelectedBar() - yoffset;

	if (num < 0)
		num = 8;
	else if (num > 8)
		num = 0;

	hotbar->setSelectedBar(num);
}

// GLFW_MOUSE_BUTTON_: LEFT, RIGHT, MIDDLE
bool Main::getMouseInput(int button) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		return true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		return true;
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		return true;

	return false;
}


vector Main::getMouseBlockPos() {
	float yOffset = circle1->getRadius();
	vector mouseWorldPos = math::screenToWorld(mousePos);
	vector mouseBlockPos = { floor(mouseWorldPos.x / blockSize) * blockSize, floor((mouseWorldPos.y - yOffset) / blockSize) * blockSize + yOffset };
	return mouseBlockPos;
}

vector Main::getCharacterBlockPos() {
	float yOffset = circle1->getRadius();
	vector worldPos = character->getLoc();
	vector blockPos = { floor(worldPos.x / blockSize) * blockSize, floor((worldPos.y - yOffset) / blockSize) * blockSize + yOffset };
	return blockPos;
}

vector Main::getBlockPos(vector pos) {
	float yOffset = circle1->getRadius();
	vector worldPos = pos;
	vector blockPos = { floor(worldPos.x / blockSize) * blockSize, floor((worldPos.y - yOffset) / blockSize) * blockSize + yOffset };
	return blockPos;
}

void Main::breakingBlock() {
	if (!isBreaking && selectedBlock) {
		std::cout << "breaking block" << std::endl;
		isBreaking = true;
		breakTimer->start(1);
		breakAnim->setLoc(selectedBlock->pos / pixelSize);
		breakAnim->start();
	}
}

void Main::cancelBlockBreak() {
	breakAnim->stop();
	breakTimer->stop();
	isBreaking = false;
}

void Main::breakBlock() {
	hotbar->addItemToHotbar(blocks[selectedBlock->posInChunk].textureIndex);
	blocks[selectedBlock->posInChunk].textureIndex = 0;
	updateChunk();

	client->changeBlockAtIndex(selectedBlock->posInChunk, 0);
}

// do not need to send in block pos, will do it for you
long long Main::getBlockIndexFromWorldPos(vector pos) {
	vector blockPos = getBlockPos(pos);

	// temp
	blockSize;

	// left block: 0, right block: chunkSize.x * blockSize

	// what is top left of chunk
	vector chunkLoc = getChunkPos();

	float x = (blockPos.x - chunkLoc.x) / blockSize;
	float y = (chunkLoc.y - blockPos.y) / blockSize;

	if (x < 0 || x > chunkSize.x || y < 0 || y > chunkSize.y)
		return -1;

	return y * chunkSize.x + x;
}

long long Main::solidBlockBelowCharacter() {
	// can fix this function to find stuff way fast,
	// if i know the location and size of the chunk then i could just do simple math

	// temp
	int chunkLoc = 0;

	vector blockBelow = getCharacterBlockPos() - vector{ 0, blockSize };
	for (auto it = blocks.begin(); it != blocks.end(); ++it) {
		if (blockBelow.x == it->position.x && blockBelow.y >= it->position.y && it->textureIndex != 0) {
			return std::distance(blocks.begin(), it);
		}
	}

	return -1;
}

// change collision to aabb method
// just check all the blocks that are close to the player
void Main::testCollision() {
	/*
	float totalRadius = character->getRadius() + circle1->getRadius();
	float distance = math::distance(circle1->getLoc(), character->getLoc());
	if (distance < totalRadius) { // if touching
		// push back
		vector normal = math::normalize(circle1->getLoc() - character->getLoc()) * -1.f;
		//float depth = (totalRadius - distance) / deltaTime;
		float depth = totalRadius - distance;
		vector correction = normal * depth;

		//character->setLoc(character->getLoc() + (normal + depth));
		character->setLoc(character->getLoc() + correction);
		character->setVelocity({ 0, 0 });
	}
	return;
	*/

	// only tests the collision in the 4 blocks around the player
	vector charBlockPos = getCharacterBlockPos();
	long long index = getBlockIndexFromWorldPos(character->getLoc());
	if (index == -1)
		return;

	BlockInstance block = blocks[index];


	//long long blockBelowChar = solidBlockBelowCharacter();

	float yLoc = (getChunkPos().y - charBlockPos.y) / blockSize;
	float xLoc = (charBlockPos.x - getChunkPos().x) / blockSize;

	// different outcomes
	// { -1, 0 } left: xLoc
	// { 1, 0 } right: chunkSize.x - xLoc
	// { 0, 1 } down: chunkSize.y - yLoc
	// { 0, -1 } up: yLoc

	std::vector<vector> dir = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }; // right, left, up down
	for (int j = 0; j < dir.size(); j++) {
		// temp
		vector tempChunkSideDist;
		float useBlockSize = 0;
		if (dir[j] == vector{ -1, 0 }) { // left
			tempChunkSideDist = { xLoc, 0 };
			useBlockSize = blockSize;
		} else if (dir[j] == vector{ 0, -1 }) { // down
			tempChunkSideDist = { 0, chunkSize.y - yLoc };
			useBlockSize = blockSize;
		} else if (dir[j] == vector{ 1, 0 }) // right
			tempChunkSideDist = { chunkSize.x - xLoc, 0 };
		else if (dir[j] == vector{ 0, 1 }) // up
			tempChunkSideDist = { 0, yLoc };

		vector something = character->getLoc() + dir[j] * float(1) * blockSize;
		long long blockDirIndex1 = getBlockIndexFromWorldPos(character->getLoc() + dir[j] * float(1) * blockSize);

		// find solid block to the direction of char
		float biggestDist = math::max(tempChunkSideDist.x, tempChunkSideDist.y);

		long long blockDirChar = -1;
		for (int i = 0; i < biggestDist; i++) {
			// should get index of block next to the player, dpeending on player pos and dir
			long long blockDirIndex = getBlockIndexFromWorldPos(character->getLoc() + dir[j] * float(i) * blockSize);
			if (blockDirIndex != -1 && blocks[blockDirIndex].textureIndex != 0) {
				blockDirChar = blockDirIndex;
				break;
			}
		}

		if (blockDirChar != -1 && blocks[blockDirChar].textureIndex != -1) {
			BlockInstance currBlock = blocks[blockDirChar];
			vector newtestPos = character->getLoc() * dir[j] * -1;
			vector newtestPos1 = (vector{ currBlock.position.x, currBlock.position.y } + (dir[j] * -1 * character->getRadius()) + useBlockSize) * dir[j] * -1;

			float temp = 1;
			if (newtestPos.y > newtestPos1.y)
				temp = -1;
			else if (newtestPos.x > newtestPos1.x)
				temp = -1;

			float depth = math::distance(newtestPos, newtestPos1) * temp;
			if (depth > 0) {
				character->setLoc(character->getLoc() + dir[j] * -1 * depth);
				//character->setVelocity(vector{ 0, 0 });
				vector absDir = { abs(dir[j].y), abs(dir[j].x) };
				character->setVelocity(absDir * character->getVelocity());
			}
		}
	}
}

// temp
vector Main::getChunkPos() {
	// temp
	float topChunkPos = -blockSize + circle1->getRadius() + 256.f * blockSize;
	return { 0, topChunkPos };
}

void Main::updateChunk() {
	// update the instance buffer
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, blocks.size() * sizeof(BlockInstance), &blocks[0]);
}

void Main::playerJoined(int id) {
	Acharacter* currChar = new Acharacter(15.f);
	currChar->setLoc(circle1->getLoc() + vector{400, (circle1->getRadius() + currChar->getRadius() + 20)});
	otherCharacters.insert({ id, currChar });
	std::cout << "player has joined: " << id << std::endl;
}

void Main::playerDisconnected(int id) {
	//otherCharacters.erase(id);
}

void Main::addPreviousPlayers(std::map<int, clientData*> clientMap) {
	//std::cout << "clientMap: " << clientMap[0].GetID() << std::endl;
	// need to get this from the server?
}