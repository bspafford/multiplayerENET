#pragma once

// #include "timer.h"

#include <string>
#include <vector>
#include <cassert>
#include <functional>

#include "math.h"

class timer;
class img;
class Shader;

struct animDataStruct {
	vector start; // x and y where frame starts (0, 0) is top left
	vector end; // x and y where frame ends
	float duration; // how fast the animation is
	bool loop; // if the animation loops
};

class animation {
public:
	// if usecharLoc, then object is in world space
	// sprite sheet path already inside image folder, no need to use "./images/" anymore
	// animation sheet only works with animations being on one row at a time, cant have half an animation on the next row down
	animation(std::string spriteSheetPath, int cellWidth, int cellHeight, std::unordered_map<std::string, animDataStruct> animData, bool useCharLoc = true, vector loc = { 0, 0 });

	//animation(float fps, std::vector<std::string> animList, Fimg* currSprite, bool loop, vector loc = {0, 0});
	~animation();

	void draw(Shader* shaderProgram);

	void setLoc(vector loc);
	vector getLoc();

	void start();
	void stop();

	void setAnimation(std::string name, bool instantUpdate = true);

	void animCallBack();

	template <class T> void addAnimEvent(int frameNum, T* const object, void (T::* const callback) ()) {
		eventFrameNum = frameNum;
		eventCallback_ = std::bind_front(callback, object);
	}

	template <class T> void addFinishedCallback(T* const object, void (T::* const callback) ()) {
		finishedCallback_ = std::bind_front(callback, object);
	}

	// calls function every animation frame
	template <class T> void addFrameCallback(T* const object, void (T::* const callback) (int)) {
		frameCallback_ = std::bind(callback, object, std::placeholders::_1);
	}

	int eventFrameNum;
	std::function<void()> eventCallback_ = NULL;
	std::function<void()> finishedCallback_ = NULL;
	std::function<void(int)> frameCallback_ = NULL;

	bool finished();

	bool bFinished = false;
	bool bStopped = false;


private:
	vector loc;
public:
	timer* animTimer;

	// new stuff
	int cellWidth, cellHeight;
	int cellWidthNum, cellHeightNum;
	std::unordered_map<std::string, animDataStruct> animData;
	img* spriteSheet;
	bool useCharLoc;

	std::string currAnim; // which animation is currently selected
	vector currFrameLoc; // which frame the animation is on
	Rect source;

	// calculates what frame the animation is on
	// if getFrameNum = true, then function finds distance from start to frame
	// if false, then the function finds distance from start to end
	int calcFrameDistance(bool getFrameNum);
};
