#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;
layout (location = 2) in float aTexIndex;

out vec3 TexCoord;

uniform mat4 camMatrix;
uniform bool useWorldPos;

void main() {
    if (useWorldPos) {
        vec4 worldPosition = vec4(aPos + aOffset, 0.0, 1.0);
        gl_Position = camMatrix * worldPosition;
    } else {
        gl_Position = camMatrix * vec4(aPos + aOffset, 0, 1);
    }

    float textureIndex = aTexIndex;
    float atlasWidth = 64.f;
    float tielWidth = 16.f;
    float tileWidth = 16.f;
    float tileHeight = 16.f;
    float scale = 16.f * 3.f;

    // Calculate the offset for the current texture
    float xOffset = mod(textureIndex, atlasWidth) * tileWidth;
    float yOffset = floor(textureIndex / atlasWidth) * tileHeight;
    //TexCoord = vec3(1 - (aPos) / scale, int(aTexIndex));
    vec2 scaledPos = aPos / scale;
    TexCoord = vec3(vec2(scaledPos.x, 1 - scaledPos.y), int(aTexIndex));
}