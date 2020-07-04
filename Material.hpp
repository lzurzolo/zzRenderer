//
// Created by lorenzo on 7/4/20.
//

#ifndef ZZRENDERER_MATERIAL_HPP
#define ZZRENDERER_MATERIAL_HPP

#include <string>
#include <glm/glm.hpp>
#include "ShaderUniform.hpp"

struct PBRMetallicRoughness
{
    PBRMetallicRoughness()
    : metallicFactor(0.0f)
    , baseColorFactor(glm::vec4{0.0f, 0.0f, 0.0f, 0.0f}, "pbrMetallicRoughnessBaseColor")
    {}
    float metallicFactor;
    Uniform<glm::vec4> baseColorFactor;
};


class Material
{
public:
    Material();
    ~Material();
    std::string Name() { return mName; }

private:
    std::string mName;
    PBRMetallicRoughness mMetallicRoughness;
};

#endif //ZZRENDERER_MATERIAL_HPP
