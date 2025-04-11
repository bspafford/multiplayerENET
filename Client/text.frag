#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 uColor;

void main()
{
    float alpha = texture(texture1, TexCoord).r; // Sample the texture (single channel)
    FragColor = vec4(uColor, alpha);            // Use alpha blending for smooth text
}