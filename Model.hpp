//
// Created by lorenzo on 5/31/20.
//

#ifndef ZZRENDERER_MODEL_HPP
#define ZZRENDERER_MODEL_HPP

#include <vector>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "tiny_gltf.h"
#include "Vertex3.hpp"

class Mesh
{
public:
    Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
    Mesh(std::string meshName);
    Mesh(std::vector<Vertex3> v);
    ~Mesh();

    std::vector<Vertex3>& Vertices() { return mVertices; }

private:
    std::vector<Vertex3> mVertices;
    GLuint mVBO;
    GLuint mEBO;
    GLint mDrawMode;
};

class Model
{
public:
    Model();
    Model(const std::string& modelName);
    ~Model();
private:
    std::vector<Mesh> mMeshes;
    GLuint mVAO;
    // materials
    void BindModelNodes(tinygltf::Model &model, tinygltf::Node &node);
    void BindMesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
};



#endif //ZZRENDERER_MODEL_HPP
