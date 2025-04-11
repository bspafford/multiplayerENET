#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <chrono>
#define NOMINMAX // this is to not define min and max as macros, so i doesn't override my math functions
#include <enet/enet.h>
#include <map>

#include "math.h"

class Shader;
class Camera;
class circle;
class Acharacter;
class img;
class block;
class animation;
class timer;
class Uhotbar;
class Client;
class clientData;

struct blockStruct {
	vector pos;
	float blockId = 0;
	long long posInChunk = 0;
};

struct BlockInstance {
	glm::vec2 position;
	float textureIndex; // Each block gets a different texture from the atlas
};

class Main {
public:
	int createWindow();
	void Start();
	void Update(float deltaTime);
	void draw(Shader* shaderProgram);

	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static inline bool KEYS[348];
	static inline vector screenSize = { 800, 600 };

	static inline bool isGravityEnabled = true;

	static inline Camera* camera;
	static inline Acharacter* character;
	static inline std::map<int, Acharacter*> otherCharacters;
	static inline circle* circle1;
	static inline animation* breakAnim;

	static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static inline GLuint framebuffer, texture, rbo;

	static inline bool cameraFollow = true;

	static inline float pixelSize = 3.f;
	static inline vector mousePos;

	static inline float blockSize = 0;

	static inline img* outline;
	static inline blockStruct* selectedBlock;
	static inline blockStruct prevSelectedBlock;

	static inline GLFWwindow* window;
	// GLFW_MOUSE_BUTTON_: LEFT, RIGHT, MIDDLE
	static bool getMouseInput(int button);

	static unsigned char* extractTile(unsigned char* atlas, int atlasWidth, int atlasHeight,
		int tileX, int tileY, int tileWidth, int tileHeight, int channels);

	static inline GLuint VAO, VBO, instanceVBO;
	static inline std::vector<BlockInstance> blocks;

	void breakingBlock();
	void cancelBlockBreak();
	void breakBlock();
	timer* breakTimer;
	bool isBreaking = false;
	vector getMouseBlockPos();
	static vector getCharacterBlockPos();
	static vector getBlockPos(vector pos);

	// finds where a block below the character is solid, will return the index of the block, will return -1 if outside list
	static long long solidBlockBelowCharacter();

	static inline vector chunkSize = { 16, 384 };
	// do not need to send in block pos, will do it for you
	static long long getBlockIndexFromWorldPos(vector pos);

	void testCollision();

	static vector getChunkPos();

	static void updateChunk();

	static inline Uhotbar* hotbar;

	static inline Shader* textShader;

	static inline Client* client;
	static void playerJoined(int id);
	static void playerDisconnected(int id);
	// loads all the characters that came before current
	static void addPreviousPlayers(std::map<int, clientData*> clientMap);
};