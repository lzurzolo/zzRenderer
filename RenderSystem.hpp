//
// Created by lorenzo on 5/30/20.
//

#ifndef ZZRENDERER_RENDERSYSTEM_HPP
#define ZZRENDERER_RENDERSYSTEM_HPP

#include <iostream>
#include <SDL2/SDL.h>

class RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    bool Initialize();
private:
    bool CreateWindow();

    SDL_Window* mWindow;
};


#endif //ZZRENDERER_RENDERSYSTEM_HPP
