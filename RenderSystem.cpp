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
    for(auto model : mModels)
    {
        glBindVertexArray(model.VAO());
        auto meshes = model.GetMeshes();
        for(const auto& mesh : meshes)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO());
            glDrawElements(mesh.PrimitiveMode(), mesh.IndexCount(), mesh.IndexComponenType(), 0);
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

void RenderSystem::AddModel(const std::string &modelName)
{
    mModels.push_back(Model{modelName});
}

int main(int argc, char* argv[])
{
    bool running = true;
    RenderSystem rs;
    if(rs.Initialize())
    {
        const char* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "layout (location = 1) in vec3 aNormal;\n"
                                   "uniform mat4 mvp;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                   "}\0";

        const char* fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                           "    FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
                                           "}\0";

        unsigned int vertexShader;
        unsigned int fragmentShader;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        GLuint shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)rs.WindowWidth()/(float)rs.WindowHeight(), 0.1f, 100.0f);

        glm::mat4 mvp = proj * view * model;
        int mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        rs.AddModel("Box.gltf");

        ShaderProgram testShader {"basic"};

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

                glUseProgram(shaderProgram);
                rs.Draw();
                SDL_GL_SwapWindow(rs.Window());
            }
        }
    }
}