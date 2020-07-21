//
// Created by lorenzo on 7/4/20.
//

#ifndef ZZRENDERER_MATERIAL_HPP
#define ZZRENDERER_MATERIAL_HPP

#include <string>
#include <variant>
#include <glm/glm.hpp>
#include "ShaderUniform.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

typedef Uniform<glm::vec4> BaseColorFactor;

struct PBRMetallicRoughness
{
    PBRMetallicRoughness()
    : metallicFactor(0.0f, "metallicFactor")
    {}

    PBRMetallicRoughness(Uniform<float> mf, Uniform<glm::vec4> bcf, const ShaderProgram& sp)
    : metallicFactor(mf)
    , shaderProgram(sp)
    , roughnessMap(bcf)
    {
        mf.SetLocation(sp.GetUniformLocation(mf.Name()));
        bcf.SetLocation(sp.GetUniformLocation(bcf.Name()));
    }

    PBRMetallicRoughness(Uniform<float> mf, const Texture& tex, const ShaderProgram& sp)
    : metallicFactor(mf)
    , shaderProgram(sp)
    , roughnessMap(tex)
    {

    }

    void Bind() const;

    Uniform<float>              metallicFactor;
    ShaderProgram               shaderProgram;
    std::variant<BaseColorFactor, Texture> roughnessMap;
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
    void                        BindUniforms() const;

private:
    std::string                 mName;
    ShaderProgram               mCurrentShader;
    PBRMetallicRoughness        mMetallicRoughness;
};

#endif //ZZRENDERER_MATERIAL_HPP
