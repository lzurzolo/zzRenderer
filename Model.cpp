//
// Created by lorenzo on 5/31/20.
//

#include <iostream>
#include <filesystem>
#include "Model.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Mesh::Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh, const ShaderProgram& sp)
: mVBO(-1)
, mEBO(-1)
, mCurrentShader(sp)
{
    for(size_t i = 0; i < model.bufferViews.size(); ++i)
    {
        const tinygltf::BufferView &bufferView = model.bufferViews[i];
        if(bufferView.target == 0) continue;

        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

        // indices as per gltf spec
        if(bufferView.target == 34963)
        {
            glGenBuffers(1, &mEBO);
            glBindBuffer(bufferView.target, mEBO);
            glBufferData(bufferView.target, bufferView.byteLength,
                    &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
        }

        // vertices as per gltf spec
        if(bufferView.target == 34962)
        {
            glGenBuffers(1, &mVBO);
            glBindBuffer(bufferView.target, mVBO);
            glBufferData(bufferView.target, bufferView.byteLength,
                    &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
        }
    }

    // TODO : how many sets of primitives per mesh are there?
    for(size_t i = 0; i < mesh.primitives.size(); ++i)
    {
        tinygltf::Primitive primitive = mesh.primitives[i];
        for(auto& attrib : primitive.attributes)
        {
            tinygltf::Accessor accessor = model.accessors[attrib.second];
            tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
            mIndexComponentType = indexAccessor.componentType;
            mIndexCount = indexAccessor.count;
            int stride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);

            int size = 1;
            if(accessor.type != TINYGLTF_TYPE_SCALAR)
            {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first == "POSITION") vaa = 0;
            if (attrib.first == "NORMAL") vaa = 1;
            if (vaa > -1)
            {
                glEnableVertexAttribArray(vaa);
                glVertexAttribPointer(vaa, size, accessor.componentType,
                        accessor.normalized ? GL_TRUE : GL_FALSE,
                        stride, BUFFER_OFFSET(accessor.byteOffset));
            }

            mPrimitiveMode = primitive.mode;
        }

        tinygltf::Material mat = model.materials[primitive.material];

        glm::vec4 bcf = glm::vec4{0.0f, 0.0f, 0.0f, 0.0f};

        auto b = mat.pbrMetallicRoughness.baseColorFactor;

        bcf.r = b[0];
        bcf.g = b[1];
        bcf.b = b[2];
        bcf.a = b[3];

        // TODO : what happens if material name is empty
        Material m{
            mat.name,
            sp,
            PBRMetallicRoughness{
                Uniform<float>(mat.pbrMetallicRoughness.metallicFactor, "metallicFactor"),
                Uniform<glm::vec4>(glm::vec4{bcf}, "roughness"), sp}};

        mMaterial = m;
    }
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

Mesh::~Mesh()
= default;

static tinygltf::TinyGLTF gLoader;

Model::Model(const std::string& modelName, const ShaderProgram& sp)
: mModelMatrix(glm::mat4(1.0f), "model")
, mName(modelName)
, mVAO(-1)
, mCurrentShader(sp)
{
    tinygltf::Model model;
    std::string err;
    std::string warn;

    std::filesystem::path p = std::filesystem::current_path();
    p /= "../meshes/";
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

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for(auto& n : scene.nodes)
    {
        assert((n >= 0) && (n < model.nodes.size()));
        BindModelNodes(model, model.nodes[scene.nodes[n]]);
    }

    glBindVertexArray(0);

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