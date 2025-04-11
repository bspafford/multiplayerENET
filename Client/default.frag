#version 330 core
out vec4 FragColor;

in vec2 fPos;
in vec4 worldPos;

uniform vec3 color;
uniform float radius;
uniform bool isWorld;

float lerp(float min, float max, float val) {
	float dif = max - min;
	return val * dif + min;
}

float distance(vec2 a, vec2 b) {
	return sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}


void main() {
	if (isWorld) {
		float minPercent = .99985f;
		float maxPercent = 1.f;

		float dist = distance(fPos, worldPos.xy) / radius;

		float factor = smoothstep(minPercent, maxPercent, dist);

		vec3 colorInside = vec3(0.f, 0.f, 0.f);
		vec3 colorOutside = vec3(0.35686274509f, 0.5294117647f, 0.19215686274f);
		vec3 finalColor = mix(colorInside, colorOutside, factor);

		FragColor = vec4(vec3(finalColor), 1.f);
	} else {
		FragColor = vec4(color, 1.f);
	}
}