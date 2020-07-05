//
// Created by lorenzo on 7/4/20.
//

#include "Material.hpp"

Material::Material()
= default;

Material::Material(const std::string& name, const ShaderProgram& sp, const PBRMetallicRoughness& pbrmr)
: mName(name)
, mCurrentShader(sp)
, mMetallicRoughness(pbrmr)
{
    mMetallicRoughness.metallicFactor.SetLocation(mCurrentShader.GetUniformLocation(mMetallicRoughness.metallicFactor.Name()));
    mMetallicRoughness.baseColorFactor.SetLocation(mCurrentShader.GetUniformLocation(mMetallicRoughness.baseColorFactor.Name()));
}

Material::~Material()
{

}

void Material::BindUniforms() const
{
    mMetallicRoughness.metallicFactor.Bind();
    mMetallicRoughness.baseColorFactor.Bind();
}
