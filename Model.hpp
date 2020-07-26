//
// Created by lorenzo on 5/31/20.
//

#ifndef ZZRENDERER_MODEL_HPP
#define ZZRENDERER_MODEL_HPP

#include <vector>
#include <array>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>
#include "tiny_gltf.h"
#include "Vertex3.hpp"
#include "ShaderProgram.hpp"
#include "ShaderUniform.hpp"
#include "Material.hpp"
#include "zzUtil.hpp"

class Mesh
{
public:
                            Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh, const ShaderProgram& sp);
    explicit                Mesh(std::string meshName);
    explicit                Mesh(std::vector<Vertex3> v);
                            ~Mesh();

    std::vector<Vertex3>&   Vertices() { return mVertices; }
    [[nodiscard]] GLuint    EBO() const { return mEBO; };
    [[nodiscard]] GLint     PrimitiveMode() const { return mPrimitiveMode; }
    [[nodiscard]] GLint     IndexComponentType() const { return mIndexComponentType; }
    [[nodiscard]] GLint     IndexCount() const { return mIndexCount; }
    void                    SetShaderProgram(const ShaderProgram& sp) { mCurrentShader = sp; }
    void                    BindUniforms() const;
    [[nodiscard]] GLuint    VAO() const { return mVAO; }

private:
    std::vector<Vertex3>    mVertices;
    GLuint                  mVAO;
    std::array<GLuint, 4>   mVBOs;
    GLuint                  mEBO{};
    GLint                   mPrimitiveMode{};
    GLint                   mIndexComponentType{};
    GLint                   mIndexCount{};
    Material                mMaterial;
    ShaderProgram           mCurrentShader;
    void                    LoadIndexBuffers(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
    void                    LoadVertexBuffers(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
    void                    LoadMaterials(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
};

class Model
{
public:
                            Model(const std::string& modelName, const ShaderProgram& sp);
                            ~Model();

    std::vector<Mesh>&      GetMeshes() { return mMeshes; }
    Uniform<glm::mat4>      mModelMatrix;
    void                    SetShaderProgram(const ShaderProgram& sp) { mCurrentShader = sp; }
    std::string             Name() { return mName; }
    void                    BindUniforms() const;

private:
    std::string             mName;
    ShaderProgram           mCurrentShader;
    std::vector<Mesh>       mMeshes;
    void                    BindModelNodes(tinygltf::Model &model, tinygltf::Node &node);
    void                    BindMesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
};

#endif //ZZRENDERER_MODEL_HPP
