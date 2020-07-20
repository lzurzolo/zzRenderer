//
// Created by lorenzo on 7/19/20.
//

#include "Texture.hpp"

Texture::Texture(const tinygltf::Image& im, const tinygltf::Sampler& samp)
: mID(-42)
{
    glGenTextures(1, &mID);
    glBindTexture(GL_TEXTURE_2D, mID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samp.minFilter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samp.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, samp.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, samp.wrapT);

    GLenum format = GL_RGBA;

    if(im.component == 1)
    {
        format = GL_RED;
    }
    else if(im.component == 2)
    {
        format = GL_RG;
    }
    else if(im.component == 3)
    {
        format = GL_RGB;
    }
    else
    {

    }

    GLenum type = GL_UNSIGNED_BYTE;

    if(im.bits == 8)
    {

    }
    else if(im.bits == 16)
    {
        type = GL_UNSIGNED_SHORT;
    }
    else
    {

    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im.width, im.height, 0, format, type, &im.image.at(0));
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mID);
}

