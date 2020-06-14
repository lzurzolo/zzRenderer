//
// Created by lorenzo on 5/31/20.
//

#include <iostream>
#include <filesystem>
#include "Model.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Mesh::Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh)
{
    for(size_t i = 0; i < model.bufferViews.size(); ++i)
    {
        const tinygltf::BufferView &bufferView = model.bufferViews[i];
        if(bufferView.target == 0) continue;

        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
    }
}

Mesh::Mesh(std::string meshName)
{

}

Mesh::Mesh(std::vector<Vertex3> v)
{
    mVertices = v;
}

Mesh::~Mesh()
{

}

static tinygltf::TinyGLTF gLoader;

Model::Model()
= default;

Model::Model(const std::string& modelName)
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
}

Model::~Model()
{

}

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
    mMeshes.push_back({model, mesh});
}
