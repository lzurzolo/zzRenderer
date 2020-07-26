//
// Created by lorenzo on 5/31/20.
//

#include <iostream>
#include <filesystem>
#include "Model.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define POSITION 0
#define NORMAL 1
#define TEXEL 2
#define TANGENT 3

Mesh::Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh, const ShaderProgram& sp)
: mEBO(-1)
, mVAO(-1)
, mCurrentShader(sp)
{
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    const tinygltf::Primitive& primitives = mesh.primitives[0];
    mPrimitiveMode = primitives.mode;
    LoadIndexBuffers(model, primitives);
    LoadVertexBuffers(model, primitives);
    LoadMaterials(model, primitives);
    glBindVertexArray(0);
}

Mesh::Mesh(std::string meshName)
{

}

Mesh::Mesh(std::vector<Vertex3> v)
{
    mVertices = v;
}

void Mesh::BindUniforms() const
{
    mMaterial.BindUniforms();
}

void Mesh::LoadIndexBuffers(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
{
    const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
    const tinygltf::BufferView& indicesAccessorBufferView = model.bufferViews[indicesAccessor.bufferView];
    const tinygltf::Buffer& indicesBuffer = model.buffers[indicesAccessorBufferView.buffer];

    glGenBuffers(1, &mEBO);
    glBindBuffer(indicesAccessorBufferView.target, mEBO);
    glBufferData(indicesAccessorBufferView.target, indicesAccessorBufferView.byteLength,
                 &indicesBuffer.data.at(0) + indicesAccessorBufferView.byteOffset, GL_STATIC_DRAW);

    mIndexComponentType = indicesAccessor.componentType;
    mIndexCount = indicesAccessor.count;
}

void Mesh::LoadVertexBuffers(const tinygltf::Model &model, const tinygltf::Primitive &primitive)
{
    for(auto& a : primitive.attributes)
    {
        const tinygltf::Accessor& accessor = model.accessors[a.second];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
        int stride = accessor.ByteStride(bufferView);
        int size = accessor.type;

        if(a.first == "POSITION")
        {
            glGenBuffers(1, &mVBOs[POSITION]);
            glBindBuffer(bufferView.target, mVBOs[POSITION]);
            glBufferData(bufferView.target, bufferView.byteLength,
                         &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, size, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE,
                                  stride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else if(a.first == "NORMAL")
        {
            glGenBuffers(1, &mVBOs[NORMAL]);
            glBindBuffer(bufferView.target, mVBOs[NORMAL]);
            glBufferData(bufferView.target, bufferView.byteLength,
                         &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

            glEnableVertexAttribArray(NORMAL);
            glVertexAttribPointer(NORMAL, size, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE,
                                  stride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else if(a.first == "TEXCOORD_0")
        {
            glGenBuffers(1, &mVBOs[TEXEL]);
            glBindBuffer(bufferView.target, mVBOs[TEXEL]);
            glBufferData(bufferView.target, bufferView.byteLength,
                         &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

            glEnableVertexAttribArray(TEXEL);
            glVertexAttribPointer(TEXEL, size, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE,
                                  stride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else if(a.first == "TANGENT")
        {
            glGenBuffers(1, &mVBOs[TANGENT]);
            glBindBuffer(bufferView.target, mVBOs[TANGENT]);
            glBufferData(bufferView.target, bufferView.byteLength,
                         &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

            glEnableVertexAttribArray(TANGENT);
            glVertexAttribPointer(TANGENT, size, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE,
                                  stride, BUFFER_OFFSET(accessor.byteOffset));
        }
    }
}

void Mesh::LoadMaterials(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
{
    tinygltf::Material mat = model.materials[primitive.material];

    auto pbrMetallicRoughness = mat.pbrMetallicRoughness;

    // there is a texture for this material, load it
    if(pbrMetallicRoughness.baseColorTexture.index > -1)
    {
        int textureIndex = pbrMetallicRoughness.baseColorTexture.index;
        const tinygltf::Texture &tex = model.textures[textureIndex];
        const tinygltf::Image &image = model.images[tex.source];
        tinygltf::Sampler samp = model.samplers[tex.sampler];

        Texture texture {image, samp};

        Material m{
                mat.name,
                mCurrentShader,
                PBRMetallicRoughness{
                        Uniform<float>(pbrMetallicRoughness.metallicFactor, "metallicFactor"),
                        texture,
                        mCurrentShader}};

        mMaterial = m;
    }
    else
    {
        glm::vec4 bcf = glm::vec4{0.0f, 0.0f, 0.0f, 0.0f};

        auto b = pbrMetallicRoughness.baseColorFactor;

        bcf.r = b[0];
        bcf.g = b[1];
        bcf.b = b[2];
        bcf.a = b[3];

        // TODO : what happens if material name is empty
        Material m{
                mat.name,
                mCurrentShader,
                PBRMetallicRoughness{
                        Uniform<float>(pbrMetallicRoughness.metallicFactor, "metallicFactor"),
                        Uniform<glm::vec4>(glm::vec4{bcf}, "roughnessMap"),
                        mCurrentShader}};

        mMaterial = m;
    }
}

Mesh::~Mesh()
= default;

static tinygltf::TinyGLTF gLoader;

Model::Model(const std::string& modelName, const ShaderProgram& sp)
: mModelMatrix(glm::mat4(1.0f), "model")
, mName(modelName)
, mCurrentShader(sp)
{
    auto modelNameTokenized = zzUtil::SplitString(modelName, '.');
    std::string subDir = modelNameTokenized[0];

    tinygltf::Model model;
    std::string err;
    std::string warn;

    std::filesystem::path p = std::filesystem::current_path();
    p /= "../meshes/";
    p /= subDir;
    p /= modelName;

    bool ret = gLoader.LoadASCIIFromFile(&model, &err, &warn, p);

    if(!warn.empty())
    {
        std::cout << "Warning: " << warn << std::endl;
    }

    if(!err.empty())
    {
        std::cout << "Error: " << err << std::endl;
    }

    if(!ret)
    {
        std::cout << "Could not parse " << modelName << std::endl;
        return;
    }

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for(auto& n : scene.nodes)
    {
        assert((n >= 0) && (n < model.nodes.size()));
        BindModelNodes(model, model.nodes[scene.nodes[n]]);
    }

    mModelMatrix.SetLocation(sp.GetUniformLocation(mModelMatrix.Name()));
}

Model::~Model()
= default;

void Model::BindModelNodes(tinygltf::Model &model, tinygltf::Node &node)
{
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
    {
        BindMesh(model, model.meshes[node.mesh]);
    }

    for (int n : node.children)
    {
        assert((n >= 0) && (n < model.nodes.size()));
        BindModelNodes(model, model.nodes[n]);
    }
}

void Model::BindMesh(tinygltf::Model &model, tinygltf::Mesh &mesh)
{
    mMeshes.push_back({model, mesh, mCurrentShader});
}

void Model::BindUniforms() const
{
    mModelMatrix.Bind();
}