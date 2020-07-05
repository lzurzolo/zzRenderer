//
// Created by lorenzo on 5/30/20.
//

#include "RenderSystem.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderSystem::RenderSystem()
: mWindowHeight(0)
, mWindowWidth(0)
, mWindow(nullptr)
, mContext(nullptr)
{

}

RenderSystem::~RenderSystem()
{
    DestroyWindow();
    KillAPI();
    ClearResources();
}

bool RenderSystem::Initialize()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindowWidth = 1024;
    mWindowHeight = 768;

    bool windowSuccess = CreateWindow();
    if(!windowSuccess)
    {
        std::cout << "Failed to create window" << std::endl;
        return false;
    }

    bool contextSuccess = CreateContext();
    if(!contextSuccess)
    {
        std::cout << "Failed to create context" << std::endl;
        return false;
    }

    bool apiSuccess = InitAPI();
    if(!apiSuccess)
    {
        std::cout << "Failed to initialize API" << std::endl;
        return false;
    }

    return true;
}

bool RenderSystem::CreateWindow()
{
    mWindow = SDL_CreateWindow("Test Window", 100, 100,
            mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
    return mWindow != nullptr;
}

bool RenderSystem::CreateContext()
{
    mContext = SDL_GL_CreateContext(mWindow);
    return mContext != NULL;
}

bool RenderSystem::InitAPI()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        std::cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
    }

    return true;
}

void RenderSystem::Draw()
{
    for(auto& rc : mRenderComponents)
    {
        rc.second.mModel->mModelMatrix.Bind();
        glBindVertexArray(rc.second.mModel->VAO());
        auto meshes = rc.second.mModel->GetMeshes();
        for(const auto& mesh : meshes)
        {
            mesh.BindUniforms();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO());
            glDrawElements(mesh.PrimitiveMode(), mesh.IndexCount(), mesh.IndexComponentType(), 0);
        }
    }
}

void RenderSystem::DestroyWindow()
{
    SDL_DestroyWindow(mWindow);
}

void RenderSystem::KillAPI()
{
    SDL_Quit();
}

Model& RenderSystem::AddModel(const std::string &modelName, const ShaderProgram& sp)
{
    Model m{modelName, sp};
    mModels.insert(std::pair(modelName, m));
    return mModels.find(modelName)->second;
}

Model& RenderSystem::GetModel(const std::string& name)
{
    auto it = mModels.find(name);
    if(it == mModels.end())
    {
        std::cout << "Model not found" << std::endl;
        EXIT_FAILURE;
    }
    else
    {
        return it->second;
    }
}

void RenderSystem::RemoveModel(const std::string &modelName)
{
    mModels.erase(modelName);
}

void RenderSystem::ClearResources()
{
    mModels.clear();
}

RenderComponent& RenderSystem::AddRenderComponent(std::string name, const RenderComponent &rc)
{
    mRenderComponents.insert(std::pair(name, rc));
    return mRenderComponents.find(name)->second;
}

RenderComponent &RenderSystem::GetRenderComponent(std::string name)
{
    auto it = mRenderComponents.find(name);
    if(it == mRenderComponents.end())
    {
        std::cout << "RenderComponent not found" << std::endl;
        EXIT_FAILURE;
    }
    else
    {
        return it->second;
    }
}

int main(int argc, char* argv[])
{
    bool running = true;
    RenderSystem rs;

    if(rs.Initialize())
    {
        ShaderSystem ss;
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)rs.WindowWidth()/(float)rs.WindowHeight(), 0.1f, 100.0f);

        ShaderProgram sp = ss.GetShader("basic");

        Model m = rs.AddModel("Box.gltf", sp);

        Uniform<glm::mat4> viewMatrix{view, "view"};
        viewMatrix.SetLocation(sp.GetUniformLocation(viewMatrix.Name()));
        viewMatrix.Bind();

        Uniform<glm::mat4> projectionMatrix{proj, "projection"};
        projectionMatrix.SetLocation(sp.GetUniformLocation(projectionMatrix.Name()));
        projectionMatrix.Bind();

        auto rc1 = rs.AddRenderComponent("box1", RenderComponent{"box1", std::make_shared<Model>(m)});
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(1.0f, 0.0f, 0.0f));
        model2 = glm::rotate(model2, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model2 = glm::scale(model2, glm::vec3(1.5f, 1.5f, 1.5f));
        rc1.mModel->mModelMatrix.Update(model2);

        auto rc2 = rs.AddRenderComponent("box2", RenderComponent{"box2", std::make_shared<Model>(m)});
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, glm::vec3(-1.0f, 0.0f, 0.0f));
        model3 = glm::rotate(model3, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        rc2.mModel->mModelMatrix.Update(model3);

        while(running)
        {
            SDL_Event event;
            if(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    running = false;
                }
                else if(event.type == SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        default:
                            break;
                    }
                }

                glViewport(0, 0, 1024, 768);
                glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                sp.Use();
                viewMatrix.Bind();
                projectionMatrix.Bind();
                rs.Draw();
                SDL_GL_SwapWindow(rs.Window());
            }
        }
    }
}