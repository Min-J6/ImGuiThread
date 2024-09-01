#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class ImageLoader {
public:
    ImageLoader() : texture(0), width(0), height(0), nrChannels(0) {}

    ~ImageLoader() {
        if (texture) {
            glDeleteTextures(1, &texture);
        }
    }

    bool loadImage(const char* path);

    GLuint getTexture() const { return texture; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    GLuint texture;
    int width, height, nrChannels;
};