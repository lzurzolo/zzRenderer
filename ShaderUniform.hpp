//
// Created by lorenzo on 6/26/20.
//

#ifndef ZZRENDERER_SHADERUNIFORM_HPP
#define ZZRENDERER_SHADERUNIFORM_HPP

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <class T>
class Uniform
{
public:
    explicit    Uniform(T d, const std::string& name);
    void        Update(T d);
    void        Bind() const;
    void        SetLocation(GLint loc) { mLocation = loc; }
    void        PrintData();
    std::string Name() { return mName; }

private:
    T           mData;
    GLint       mLocation;
    std::string mName;
};

#endif //ZZRENDERER_SHADERUNIFORM_HPP
