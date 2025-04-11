#version 330 core
layout (location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 camMatrix;
uniform bool useWorldPos;
uniform mat4 screenProjMatrix;

void main() {
    if (useWorldPos) {
        vec4 worldPosition = vec4(aPos, 0.0, 1.0);
        gl_Position = camMatrix * worldPosition;
    } else {
        gl_Position = screenProjMatrix * vec4(aPos, 0, 1);
    }

    TexCoord = aTexCoord;
}