//
// Created by lorenzo on 7/4/20.
//

#include "Material.hpp"

void PBRMetallicRoughness::Bind() const
{
    auto* bcf = std::get_if<BaseColorFactor>(&roughnessMap);
    if(bcf)
    {
        bcf->Bind();
    }

    auto* tex = std::get_if<Texture>(&roughnessMap);
    if(tex)
    {
        tex->Bind();
    }

    metallicFactor.Bind();
}

Material::Material()
= default;

Material::Material(const std::string& name, const ShaderProgram& sp, const PBRMetallicRoughness& pbrmr)
: mName(name)
, mCurrentShader(sp)
, mMetallicRoughness(pbrmr)
{

}

Material::~Material()
{

}

void Material::BindUniforms() const
{
    mMetallicRoughness.Bind();
}


