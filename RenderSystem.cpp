//
// Created by lorenzo on 5/30/20.
//

#include "RenderSystem.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderSystem::RenderSystem()
= default;

RenderSystem::~RenderSystem()
{
    DestroyWindow();
    KillAPI();
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
    for(auto& model : mModels)
    {
        glBindVertexArray(model.second.VAO());
        auto meshes = model.second.GetMeshes();
        for(const auto& mesh : meshes)
        {
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

int main(int argc, char* argv[])
{
    bool running = true;
    RenderSystem rs;
    ShaderSystem ss;

    if(rs.Initialize())
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)rs.WindowWidth()/(float)rs.WindowHeight(), 0.1f, 100.0f);

        ShaderProgram testShader {"basic"};
        testShader.Use();

        Model m = rs.AddModel("Box.gltf", ss.GetShader("basic"));

        m.mModelMatrix.Update(model);
        m.mModelMatrix.SetLocation(testShader.GetUniformLocation("model"));
        m.mModelMatrix.Bind();

        Uniform<glm::mat4> viewMatrix{view};
        viewMatrix.SetLocation(testShader.GetUniformLocation("view"));
        viewMatrix.Bind();

        Uniform<glm::mat4> projectionMatrix{proj};
        projectionMatrix.SetLocation(testShader.GetUniformLocation("projection"));
        projectionMatrix.Bind();

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

                testShader.Use();
                m.mModelMatrix.Bind();
                viewMatrix.Bind();
                projectionMatrix.Bind();
                rs.Draw();
                SDL_GL_SwapWindow(rs.Window());
            }
        }
    }
}