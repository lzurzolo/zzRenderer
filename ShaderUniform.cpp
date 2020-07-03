//
// Created by lorenzo on 6/26/20.
//

#include "ShaderUniform.hpp"

#include <utility>

template<class T>
Uniform<T>::Uniform(T d, const std::string& name)
: mData(d)
, mLocation(0)
, mName(name)
{

}

template<class T>
void Uniform<T>::Update(T d)
{
    mData = d;
}

template<class T>
void Uniform<T>::Bind()
{
    glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(mData));
}

template class Uniform<glm::mat4>;
