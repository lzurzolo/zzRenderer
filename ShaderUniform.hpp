//
// Created by lorenzo on 6/26/20.
//

#ifndef ZZRENDERER_SHADERUNIFORM_HPP
#define ZZRENDERER_SHADERUNIFORM_HPP

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <class T>
class Uniform
{
public:
    explicit    Uniform(T d);
    void        Update(T d);
    void        Bind();
    void        SetLocation(GLint loc) { mLocation = loc; }

private:
    T           mData;
    GLint       mLocation;
};

#endif //ZZRENDERER_SHADERUNIFORM_HPP
