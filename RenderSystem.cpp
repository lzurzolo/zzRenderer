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
    return false;
}

bool RenderSystem::CreateWindow()
{
    mWindow = SDL_CreateWindow("Test Window", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
    return mWindow != nullptr;
}

int main(int argc, char* argv[])
{
    RenderSystem rs;
    rs.Initialize();
}