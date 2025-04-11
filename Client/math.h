#pragma once

#include <iostream>

struct vector {
	float x = 0;
	float y = 0;
};

struct Rect {
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;
};

// operator overloading
inline vector operator+ (vector a, vector b) {
	return { a.x + b.x, a.y + b.y };
}

inline vector operator+ (vector a, float b) {
	return { a.x + b, a.y + b };
}

inline vector operator- (vector a, vector b) {
	return { a.x - b.x, a.y - b.y };
}

inline vector operator- (vector a, float b) {
	return { a.x - b, a.y - b };
}

inline vector operator* (vector a, float b) {
	return { a.x * b, a.y * b };
}

inline vector operator* (vector a, vector b) {
	return { a.x * b.x, a.y * b.y };
}

inline vector operator*= (vector a, float b) {
	return { a.x * b, a.y * b };
}

inline vector operator/ (vector a, float b) {
	return { a.x / b, a.y / b };
}

inline std::ostream& operator<< (std::ostream& os, vector& a) {
	os << a.x << ", " << a.y;
	return os;
}

inline bool operator== (vector a, vector b) {
	return a.x == b.x && a.y == b.y;
}

class math {
public:
	static float dot(vector a, vector b);
	static vector normalize(vector a);
	static float length(vector value);
	static float distance(float x1, float y1, float x2, float y2);
	static float distance(vector a, vector b);
	static float min(float a, float b);
	static float max(float a, float b);
	static float lerp(float min, float max, float val);
	static float randRange(float min, float max);
	static float clamp(float x, float min, float max);

	// quick so its approximation
	static float Q_sqrt(float number);
	// does the inverse (1/sqrt)
	static float Q_rsqrt(float number);

	static vector screenToWorld(vector screenPos);
};