//
// Created by lorenzo on 5/30/20.
//

#ifndef ZZRENDERER_RENDERSYSTEM_HPP
#define ZZRENDERER_RENDERSYSTEM_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

class RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    bool Initialize();
    SDL_Window* Window() { return mWindow; }
private:
    bool CreateWindow();
    bool CreateContext();

    SDL_Window* mWindow;
    SDL_GLContext mContext;
};


#endif //ZZRENDERER_RENDERSYSTEM_HPP
