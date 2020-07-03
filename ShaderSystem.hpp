//
// Created by lorenzo on 6/28/20.
//

#ifndef ZZRENDERER_SHADERSYSTEM_HPP
#define ZZRENDERER_SHADERSYSTEM_HPP
#include <filesystem>
#include "ShaderProgram.hpp"

typedef std::string ShaderName;

class ShaderSystem
{
public:
    ShaderSystem();
    ~ShaderSystem();
    ShaderProgram& GetShader(ShaderName name);
    void AddShader(ShaderProgram program);
    int GetShaderCount() { return mShaders.size(); }

private:
    std::map<ShaderName, ShaderProgram> mShaders;

};
#endif //ZZRENDERER_SHADERSYSTEM_HPP
