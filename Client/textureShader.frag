#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);

    vec3 textColor = vec3(1);
    float alpha = texture(texture1, TexCoord).r; // Sample the texture (single channel)
    //FragColor = vec4(textColor, alpha);            // Use alpha blending for smooth text
}