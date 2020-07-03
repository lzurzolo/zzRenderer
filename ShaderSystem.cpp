//
// Created by lorenzo on 6/28/20.
//

#include "ShaderSystem.hpp"

ShaderSystem::ShaderSystem()
{
    std::filesystem::path p = std::filesystem::current_path();
    p /= "../shaders/";
    for(const auto& e : std::filesystem::directory_iterator(p))
    {
        if(e.is_directory())
        {
            std::string shaderName = e.path().stem().string();
            mShaders.insert(std::pair(shaderName, ShaderProgram{shaderName}));
        }
    }
}

ShaderProgram& ShaderSystem::GetShader(ShaderName name)
{
    auto it = mShaders.find(name);
    if(it == mShaders.end())
    {
        std::cout << "Shader does not exist" << std::endl;
        EXIT_FAILURE;
    }
    else
    {
        return it->second;
    }
}

void ShaderSystem::AddShader(ShaderProgram program)
{
    auto programName = program.Name();
    auto it = mShaders.find(programName);
    if(it == mShaders.end())
    {
        mShaders.insert(std::pair(programName, program));
    }
}

ShaderSystem::~ShaderSystem()
= default;

