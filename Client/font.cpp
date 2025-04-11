#define STB_TRUETYPE_IMPLEMENTATION

#include "font.h"
#include "main.h"
#include "shaderClass.h"
#include "img.h"

// Function to load a font and create a texture atlas
GLuint Font::loadFont(const char* fontPath, float fontSize, stbtt_bakedchar* cdata) {
    // Load the font using stb_truetype
    FILE* fontFile = fopen(fontPath, "rb");
    if (!fontFile) {
        std::cerr << "Error opening font file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    fseek(fontFile, 0, SEEK_END);
    long fontFileSize = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);

    unsigned char* fontBuffer = new unsigned char[fontFileSize];
    fread(fontBuffer, 1, fontFileSize, fontFile);
    fclose(fontFile);

    stbtt_fontinfo fontInfo;
    if (!stbtt_InitFont(&fontInfo, fontBuffer, 0)) {
        std::cerr << "Error initializing font!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create a bitmap to store the font texture
    const int atlasWidth = 512, atlasHeight = 512;
    unsigned char* bitmap = new unsigned char[atlasWidth * atlasHeight];
    stbtt_BakeFontBitmap(fontBuffer, 0, fontSize, bitmap, atlasWidth, atlasHeight, 32, 96, cdata);

    // Create an OpenGL texture from the bitmap
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete[] bitmap;

    return texture;
}

// Function to render text
void Font::renderText(GLuint shaderProgram, GLuint VAO, GLuint VBO, GLuint texture, stbtt_bakedchar* cdata, const char* text, float x, float y, float scale, glm::vec3 color) {
    glUseProgram(shaderProgram);

    glUniform3fv(glGetUniformLocation(shaderProgram, "uColor"), 1, glm::value_ptr(color));

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    float xOffset = 0;
    for (const char* p = text; *p; p++) {
        stbtt_bakedchar c = cdata[*p - 32];
        float xpos = x + xOffset;
        float ypos = y;

        float w = c.x1 - c.x0;
        float h = c.y1 - c.y0;

        size.y = math::max(h, size.y);

        float vertices[6][4] = {
            { xpos,     ypos + h,   c.x0 / 512.0f, c.y0 / 512.0f },
            { xpos,     ypos,       c.x0 / 512.0f, c.y1 / 512.0f },
            { xpos + w, ypos,       c.x1 / 512.0f, c.y1 / 512.0f },

            { xpos,     ypos + h,   c.x0 / 512.0f, c.y0 / 512.0f },
            { xpos + w, ypos,       c.x1 / 512.0f, c.y1 / 512.0f },
            { xpos + w, ypos + h,   c.x1 / 512.0f, c.y0 / 512.0f }
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        xOffset += (c.x1 - c.x0) * scale + Main::pixelSize;
    }

    size.x = xOffset;

    glBindVertexArray(0);
}
Font::Font(std::string text, std::string fontPath, float fontSize) {
    this->text = text;

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load font and create texture
    fontTexture = loadFont(fontPath.c_str(), fontSize, cdata);
}

void Font::draw() {
    renderText(Main::textShader->ID, VAO, VBO, fontTexture, cdata, text.c_str(), loc.x + Main::pixelSize, loc.y - Main::pixelSize, 1.0f, glm::vec3(.2431372549f));
    renderText(Main::textShader->ID, VAO, VBO, fontTexture, cdata, text.c_str(), loc.x, loc.y, 1.0f, glm::vec3(.98823529411f));
}

void Font::setLoc(vector loc) {
    this->loc = loc;
}

vector Font::getSize() {
    return size;
}

void Font::setText(std::string text) {
    this->text = text;
}