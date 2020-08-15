//
// Created by lorenzo on 5/31/20.
//

#include <iostream>
#include <filesystem>
#include "Model.hpp"

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

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, const ShaderProgram& sp)
: mEBO(-1)
, mVAO(-1)
, mCurrentShader(sp)
{
    bool hasTexels = true;
    if(!mesh->mTextureCoords[0])
    {
        hasTexels = false;
    }
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        auto p = mesh->mVertices[i];
        auto n = mesh->mNormals[i];

        glm::vec3 pos = glm::vec3(p.x, p.y, p.z);
        glm::vec3 normals = glm::vec3(n.x, n.y, n.z);

        glm::vec2 texels = glm::vec2(0.0f, 0.0f);
        if(hasTexels)
        {
            auto t = mesh->mTextureCoords[0][i];
            texels = glm::vec2(t.x, t.y);
        }
        mVertices.push_back({pos, normals, texels});
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            mIndices.push_back(face.mIndices[j]);
        }
    }

    LoadBuffers();
    if(scene->mNumMaterials > 0) LoadMaterials(mesh, scene);
}

Mesh::Mesh(std::string meshName)
{

}

Mesh::Mesh(std::vector<Vertex3> v, std::vector<unsigned int> i, const ShaderProgram& sp)
: mEBO(-1)
, mVAO(-1)
, mCurrentShader(sp)
{
    mVertices = v;
    mIndices = i;

    LoadBuffers();
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
                                  stride, (void*)accessor.byteOffset);
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
                                  stride, (void*)accessor.byteOffset);
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
                                  stride, (void*)accessor.byteOffset);
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
                                  stride, (void*)accessor.byteOffset);
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

void Mesh::LoadMaterials(aiMesh* mesh, const aiScene* scene)
{
    auto mat =  scene->mMaterials[mesh->mMaterialIndex];
    aiString str;
    auto albedo = mat->GetTexture(aiTextureType_BASE_COLOR, 0, &str);
    auto ao = mat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &str);
    printf("%s\n", str.C_Str());
    std::cout << str.C_Str() << std::endl;
    // base color/albedo
    // normal map
    // roughness
    // height map
    // metalness
    // occlusion
    // emission
}

void Mesh::LoadBuffers()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex3), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    mIndexCount = mIndices.size();

    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)0);
    glEnableVertexAttribArray(NORMAL);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, normal));
    glEnableVertexAttribArray(TEXEL);
    glVertexAttribPointer(TEXEL, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, texel));
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
    std::string extension = modelNameTokenized[1];

    std::filesystem::path p = std::filesystem::current_path();
    p /= "../meshes/";
    p /= subDir;
    p /= modelName;

    if(extension == "gltf")
    {
        tinygltf::Model model;
        std::string err;
        std::string warn;

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

        LoadGLTF(model);
    }
    else
    {
        Assimp::Importer importer;
        LoadAssimp(importer, importer.ReadFile(p, aiProcess_Triangulate | aiProcess_FlipUVs));
    }



    mModelMatrix.SetLocation(sp.GetUniformLocation(mModelMatrix.Name()));
}

Model::~Model()
= default;

void Model::BindModelNodes(tinygltf::Model &model, tinygltf::Node &node)
{
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
    {
        BindMeshGLTF(model, model.meshes[node.mesh]);
    }

    for (int n : node.children)
    {
        assert((n >= 0) && (n < model.nodes.size()));
        BindModelNodes(model, model.nodes[n]);
    }
}

void Model::BindMeshGLTF(tinygltf::Model &model, tinygltf::Mesh &mesh)
{
    mMeshes.push_back({model, mesh, mCurrentShader});
}

void Model::BindUniforms() const
{
    mModelMatrix.Bind();
}

void Model::LoadGLTF(tinygltf::Model& model)
{
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];

    for(auto& n : scene.nodes)
    {
        assert((n >= 0) && (n < model.nodes.size()));
        BindModelNodes(model, model.nodes[scene.nodes[n]]);
    }
}

void Model::LoadAssimp(const Assimp::Importer& importer, const aiScene* scene)
{
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Assimp error" << importer.GetErrorString() << std::endl;
        EXIT_FAILURE;
    }

    BindModelNodesAssimp(scene->mRootNode, scene);
}

void Model::BindModelNodesAssimp(aiNode* node, const aiScene* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh m {mesh, scene, mCurrentShader};
        mMeshes.push_back(m);
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        BindModelNodesAssimp(node->mChildren[i], scene);
    }
}