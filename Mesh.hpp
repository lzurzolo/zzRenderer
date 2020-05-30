//
// Created by lorenzo on 5/30/20.
//

#ifndef ZZRENDERER_MESH_HPP
#define ZZRENDERER_MESH_HPP

#include <vector>
#include "Vertex3.hpp"
class Mesh
{
public:
    Mesh();
    Mesh(std::vector<Vertex3> v);
    ~Mesh();

    std::vector<Vertex3>& Vertices() { return mVertices; }

private:
    std::vector<Vertex3> mVertices;
};


#endif //ZZRENDERER_MESH_HPP
