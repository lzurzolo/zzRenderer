//
// Created by lorenzo on 5/30/20.
//

#ifndef ZZRENDERER_VERTEX3_HPP
#define ZZRENDERER_VERTEX3_HPP

#include <glm/glm.hpp>

struct Vertex3
{
    Vertex3(glm::vec3 p, glm::vec3 c)
    {
        position = p;
        color = c;
    }
    glm::vec3 position{};
    glm::vec3 color{};
};
#endif //ZZRENDERER_VERTEX3_HPP
