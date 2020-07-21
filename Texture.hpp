//
// Created by lorenzo on 7/19/20.
//

#ifndef ZZRENDERER_TEXTURE_HPP
#define ZZRENDERER_TEXTURE_HPP

#include <GL/glew.h>
#include "tiny_gltf.h"

class Texture
{
public:
    Texture()
    : mID(-42)
    {}
    Texture(const tinygltf::Image& im, const tinygltf::Sampler& samp);
    ~Texture() {}

    void Bind() const;

private:
    GLuint mID;
};

#endif //ZZRENDERER_TEXTURE_HPP
