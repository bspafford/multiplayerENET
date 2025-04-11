#pragma once

#include <set>
#include <iostream>
#include <cassert>
#include <functional>

class timer {
public:
	timer();
	~timer();

	static void callUpdate(float deltaTime);

	void Update(float deltaTime);

	void setFps(float fps);

	// this starts the timer
	void start(float maxTime);
	void stop();

	float getTime();
	float getMaxTime();
	bool finished();

	// sets up callback and fps
	template <class T> void addCallback(T* const object, void(T::* const finish)(), void(T::* const update)() = NULL) {
		callback_ = std::bind_front(finish, object);

		if (update != NULL)
			updateCallback_ = std::bind_front(update, object);
	}

	// works for static functions
	void addCallback(void (*callback) ()) {
		callback_ = callback;
	}

	template <class T> void addUpdateCallback(T* const object, void(T::* const update)()) {
		updateCallback_ = std::bind_front(update, object);
	}

	void addUpdateCallback(void (*callback) ()) {
		updateCallback_ = callback;
	}

private:
	// keeps track of all instances to call update function on
	static inline std::set<timer*> instances;

	float time = 0;
	float maxTime;
	bool bStart = false;
	bool bFinished = false;

	std::function<void()> callback_;
	std::function<void()> updateCallback_;
};