#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

//uniform sampler2D texture1;
uniform sampler2DArray textureAtlas; // Use a texture array for different block textures

void main()
{
    //FragColor = texture(texture1, TexCoord);
    //FragColor = texture(textureAtlas, vec3(TexCoord, TexID)); // Select texture from the atlas
    
    FragColor = texture(textureAtlas, TexCoord); // TexCoord
    
    //FragColor = vec4(1.f);
}