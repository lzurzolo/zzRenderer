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

}

template<>
void Uniform<glm::vec4>::Bind()
{
    glUniform4fv(mLocation, 1, glm::value_ptr(mData));
}

template<>
void Uniform<glm::mat4>::Bind()
{
    glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(mData));
}

template class Uniform<glm::vec4>;
template class Uniform<glm::mat4>;
