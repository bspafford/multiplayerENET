#version 330 core
layout (location = 0) in vec2 aPos;  // 2D Position of the vertex

out vec2 fPos;
out vec4 worldPos;

uniform mat4 camMatrix;
uniform vec2 pos;
//uniform vec2 camPos;

void main() {
    gl_Position = camMatrix * vec4(aPos + pos, 0.0, 1.0); // Convert to 4D vector
    fPos = pos;
    worldPos = vec4(aPos + pos, 0.f, 1.f); //inverse(camMatrix) * vec4(aPos, 0.f, 1.f);
}