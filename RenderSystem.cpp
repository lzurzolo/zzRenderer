//
// Created by lorenzo on 5/30/20.
//

#include "RenderSystem.hpp"

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
    mWindow = SDL_CreateWindow("Test Window", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
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

void RenderSystem::DestroyWindow()
{
    SDL_DestroyWindow(mWindow);
}

void RenderSystem::KillAPI()
{
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    bool running = true;
    RenderSystem rs;
    if(rs.Initialize())
    {
        std::vector<Vertex3> vertices;
        vertices.push_back(Vertex3{glm::vec3{-0.5f, 0.0f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}});
        vertices.push_back(Vertex3{glm::vec3{0.0f, 0.5f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}});
        vertices.push_back(Vertex3{glm::vec3{0.5f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}});

        std::string mod = "Box.gltf";

        Model test {mod};
        Mesh triangle {vertices};

        GLuint VAO;
        GLuint VBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, triangle.Vertices().size() * sizeof(Vertex3), triangle.Vertices().data(), GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)0);
        glEnableVertexAttribArray(0);

        // color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, color));
        glEnableVertexAttribArray(1);

        const char* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                   "}\0";

        const char* fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                           "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                SDL_GL_SwapWindow(rs.Window());
            }
        }
    }
}