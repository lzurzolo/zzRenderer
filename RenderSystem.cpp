//
// Created by lorenzo on 5/30/20.
//

#include "RenderSystem.hpp"

RenderSystem::RenderSystem()
= default;

RenderSystem::~RenderSystem()
= default;

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

int main(int argc, char* argv[])
{
    bool running = true;
    RenderSystem rs;
    if(rs.Initialize())
    {
        while(running)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_KEYDOWN)
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
                else if(event.type == SDL_QUIT)
                {
                    running = false;
                }

                glViewport(0, 0, 1024, 768);
                glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                SDL_GL_SwapWindow(rs.Window());
            }
        }
    }
}