//
// Created by lorenzo on 6/20/20.
//

#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const std::string &shaderName)
: mName(shaderName)
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

    int success;
    char infoLog[512];
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(mID, 512, NULL, infoLog);
        std::cout << "Error: shader program linking failed\n" << infoLog << std::endl;
    }

    Use();

    for(auto s : shaders)
    {
        glDeleteShader(s);
    }

    InitializeActiveUniformTable(mID);
}

ShaderProgram::~ShaderProgram()
= default;

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

    char debugLog[512];
    int success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, debugLog);
        std::cout << "Error: shader compilation failed\n" << debugLog << std::endl;
    }

    return shader;
}

void ShaderProgram::InitializeActiveUniformTable(GLuint shaderProgram)
{
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &count);

    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(mID, (GLuint)i, bufSize, &length, &size, &type, name);
        int location = glGetUniformLocation(mID, name);

        mUniforms.insert(std::pair(name, UniformInfo{name, type, location}));
    }
}

GLint ShaderProgram::GetUniformLocation(UniformName name) const
{
    auto it = mUniforms.find(name);
    if(it == mUniforms.end())
    {
        std::cout << "Uniform not present in shader" << std::endl;
        return -42;
    }
    else
    {
        return it->second.location;
    }
}

void ShaderProgram::PrintActiveUniforms()
{
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(mID, (GLuint)i, bufSize, &length, &size, &type, name);
        int location = glGetUniformLocation(mID, name);

        printf("Uniform #%d Type: %u Name: %s Location: %i\n", i, type, name, location);
    }
}

void ShaderProgram::PrintActiveAttributes()
{
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(mID, GL_ACTIVE_ATTRIBUTES, &count);
    printf("Active Attributes: %d\n", count);

    for (i = 0; i < count; i++)
    {
        glGetActiveAttrib(mID, (GLuint)i, bufSize, &length, &size, &type, name);

        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
    }
}

