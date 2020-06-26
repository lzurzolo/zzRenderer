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
#include "ShaderUniform.hpp"

class Mesh
{
public:
    Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
    explicit Mesh(std::string meshName);
    explicit Mesh(std::vector<Vertex3> v);
    ~Mesh();

    std::vector<Vertex3>& Vertices() { return mVertices; }
    [[nodiscard]] GLuint EBO() const { return mEBO; };
    [[nodiscard]] GLint PrimitiveMode() const { return mPrimitiveMode; }
    [[nodiscard]] GLint IndexComponenType() const { return mIndexComponentType; }
    [[nodiscard]] GLint IndexCount() const { return mIndexCount; }

private:
    std::vector<Vertex3> mVertices;
    GLuint mVBO;
    GLuint mEBO;
    GLint mPrimitiveMode;
    GLint mIndexComponentType;
    GLint mIndexCount;
};

class Model
{
public:
    explicit Model(const std::string& modelName);
    ~Model();
    [[nodiscard]] GLuint VAO() const { return mVAO; }
    std::vector<Mesh>& GetMeshes() { return mMeshes; }
    Uniform<glm::mat4> mModelMatrix;
private:
    std::vector<Mesh> mMeshes;
    GLuint mVAO;
    // materials
    void BindModelNodes(tinygltf::Model &model, tinygltf::Node &node);
    void BindMesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
};

#endif //ZZRENDERER_MODEL_HPP
