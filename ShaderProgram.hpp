//
// Created by lorenzo on 6/20/20.
//
#ifndef ZZRENDERER_SHADERPROGRAM_HPP
#define ZZRENDERER_SHADERPROGRAM_HPP

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

class ShaderProgram
{
public:
    ShaderProgram(const std::string& shaderName);
    ~ShaderProgram();
    void Use() { glUseProgram(mID); }

private:
    GLuint mID;
    GLuint CreateAndCompileShader(std::filesystem::path path, ShaderType st);
};
#endif //ZZRENDERER_SHADERPROGRAM_HPP
