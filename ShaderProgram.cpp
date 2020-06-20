//
// Created by lorenzo on 6/20/20.
//

#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const std::string &shaderName)
{
    /*
     * .vs vertex shader
     * .fs fragment shader
     * .gs geometry shader
     */

    std::vector<GLuint> shaders;

    std::filesystem::path p = std::filesystem::current_path();
    p /= "../shaders/";
    p /= shaderName + "/";

    for(const auto& e : std::filesystem::directory_iterator(p))
    {
        if(e.path().extension() == ".vs")
        {
            shaders.push_back(CreateAndCompileShader(e.path(), ShaderType::VERTEX));
        }
        else if(e.path().extension() == ".fs")
        {
            shaders.push_back(CreateAndCompileShader(e.path(), ShaderType::FRAGMENT));
        }
        else if(e.path().extension() == ".gs")
        {
            shaders.push_back(CreateAndCompileShader(e.path(), ShaderType::GEOMETRY));
        }
    }

    mID = glCreateProgram();

    for(auto s : shaders)
    {
        glAttachShader(mID, s);
    }

    glLinkProgram(mID);

    for(auto s : shaders)
    {
        glDeleteShader(s);
    }
}

ShaderProgram::~ShaderProgram()
{

}

GLuint ShaderProgram::CreateAndCompileShader(std::filesystem::path path, ShaderType st)
{
    GLuint shader;
    if(st == ShaderType::VERTEX)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(st == ShaderType::FRAGMENT)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else if(st == ShaderType::GEOMETRY)
    {
        shader = glCreateShader(GL_GEOMETRY_SHADER);
    }
    else
    {
        return -1;
    }

    std::stringstream buffer;
    std::fstream shaderFile(path.c_str());
    buffer << shaderFile.rdbuf();

    std::string shaderContents = buffer.str();
    const char* shaderCode = shaderContents.c_str();
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    return shader;
}

