//
// Created by lorenzo on 7/4/20.
//

#ifndef ZZRENDERER_MATERIAL_HPP
#define ZZRENDERER_MATERIAL_HPP

#include <string>
#include <glm/glm.hpp>
#include "ShaderUniform.hpp"
#include "ShaderProgram.hpp"

struct PBRMetallicRoughness
{
    PBRMetallicRoughness()
    : metallicFactor(0.0f, "metallicFactor")
    , baseColorFactor(glm::vec4{0.0f, 0.0f, 0.0f, 0.0f}, "roughness")
    {}

    PBRMetallicRoughness(Uniform<float> mf, Uniform<glm::vec4> bcf, const ShaderProgram& sp)
    : metallicFactor(mf)
    , baseColorFactor(bcf)
    , shaderProgram(sp)
    {
        mf.SetLocation(sp.GetUniformLocation(mf.Name()));
        bcf.SetLocation(sp.GetUniformLocation(bcf.Name()));
    }

    Uniform<float>              metallicFactor;
    Uniform<glm::vec4>          baseColorFactor;
    ShaderProgram               shaderProgram;
};


class Material
{
public:
                                Material();
    explicit                    Material(const std::string& name, const ShaderProgram& sp, const PBRMetallicRoughness& pbrmr);
                                ~Material();
    [[nodiscard]] std::string   Name() const { return mName; }
    void                        SetName(std::string name) { mName = name; }
    void                        SetPBRMetallicRoughness(const PBRMetallicRoughness& pbrmr) { mMetallicRoughness = pbrmr; }
    void                        BindUniforms();

private:
    std::string                 mName;
    ShaderProgram               mCurrentShader;
    PBRMetallicRoughness        mMetallicRoughness;
};

#endif //ZZRENDERER_MATERIAL_HPP
