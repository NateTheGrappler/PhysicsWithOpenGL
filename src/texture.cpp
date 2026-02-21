#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include <iostream>
#include "vendor/stb_image.h"



Texture::Texture(const std::string& filePath)
    :m_texturePath(filePath)
{
    //generate and bind the texture you have made
    glGenTextures(1, &m_renderID);
    glBindTexture(GL_TEXTURE_2D, m_renderID);

    //set a bunch of default params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //set up the image data from the path
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded texture: " << filePath << " - Width: " << m_width << ", Height: " << m_height << ", Channels: " << m_nrChannels << std::endl;

        //check to see what type of an image you have
        GLenum imageFormat = GL_RGB;
        if (m_nrChannels == 1) { imageFormat = GL_RED; }
        else if (m_nrChannels == 3) { imageFormat = GL_RGB; }
        else if (m_nrChannels == 4) { imageFormat = GL_RGBA; }

        // FIX: Use the same format for internal and external
        glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, m_width, m_height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Unable to get the image data at: " << filePath << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture()
{
    std::cout << "Deleting texture with ID: " << m_renderID << " - Path: " << m_texturePath << std::endl;
    glDeleteTextures(1, &m_renderID);
}

void Texture::setParameter(int pname, int param)
{
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_renderID);
}

void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_renderID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
