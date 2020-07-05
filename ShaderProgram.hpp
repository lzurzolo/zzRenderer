//
// Created by lorenzo on 6/20/20.
//
#ifndef ZZRENDERER_SHADERPROGRAM_HPP
#define ZZRENDERER_SHADERPROGRAM_HPP

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

typedef std::string UniformName;

struct UniformInfo
{
    UniformInfo(UniformName n, GLenum t, GLint loc)
    : name(n)
    , type(t)
    , location(loc)
    {}
    UniformName                         name;
    GLenum                              type;
    GLint                               location;
};

class ShaderProgram
{
public:
                                        ShaderProgram();
    explicit                            ShaderProgram(const std::string& shaderName);
                                        ~ShaderProgram();
    void                                Use() const { glUseProgram(mID); }
    [[nodiscard]] GLuint                ID() const { return mID; }
    std::string                         Name() const { return mName; }
    GLint                               GetUniformLocation(UniformName name) const;
    void                                PrintActiveUniforms();
    void                                PrintActiveAttributes();

private:
    GLuint                              mID;
    GLuint                              CreateAndCompileShader(std::filesystem::path path, ShaderType st);
    void                                InitializeActiveUniformTable(GLuint shaderProgram);
    std::map<UniformName, UniformInfo>  mUniforms;
    std::string                         mName;
};
#endif //ZZRENDERER_SHADERPROGRAM_HPP
