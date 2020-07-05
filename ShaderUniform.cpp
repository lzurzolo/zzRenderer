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
void Uniform<T>::Bind() const
{

}

template<class T>
void Uniform<T>::PrintData()
{

}

template<>
void Uniform<glm::vec4>::Bind() const
{
    glUniform4fv(mLocation, 1, glm::value_ptr(mData));
}

template<>
void Uniform<glm::mat4>::Bind() const
{
    glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(mData));
}

template<>
void Uniform<float>::Bind() const
{
    glUniform1f(mLocation, mData);
}

template<>
void Uniform<float>::PrintData()
{
    std::cout << mData << std::endl;
}

template<>
void Uniform<glm::vec4>::PrintData()
{
    std::cout << mData.x << ", " << mData.y << ", " << mData.z << ", " << mData.w << std::endl;
}

template<>
void Uniform<glm::mat4>::PrintData()
{
    std::cout << std::endl;
    std::cout << "[ " << mData[0][0] << ", " << mData[0][1] << ", " << mData[0][2] << ", " << mData[0][3] << " ]" << std::endl;
    std::cout << "[ " << mData[1][0] << ", " << mData[1][1] << ", " << mData[1][2] << ", " << mData[1][3] << " ]" << std::endl;
    std::cout << "[ " << mData[2][0] << ", " << mData[2][1] << ", " << mData[2][2] << ", " << mData[2][3] << " ]" << std::endl;
    std::cout << "[ " << mData[3][0] << ", " << mData[3][1] << ", " << mData[3][2] << ", " << mData[3][3] << " ]" << std::endl;
    std::cout << std::endl;
}


template class Uniform<glm::vec4>;
template class Uniform<glm::mat4>;
template class Uniform<float>;
