//
// Created by lorenzo on 5/30/20.
//

#ifndef ZZRENDERER_RENDERSYSTEM_HPP
#define ZZRENDERER_RENDERSYSTEM_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Model.hpp"
#include "ShaderProgram.hpp"

class RenderSystem
{
public:
                                            RenderSystem();
                                            ~RenderSystem();

    bool                                    Initialize();
    void                                    Draw();
    SDL_Window*                             Window() { return mWindow; }
    [[nodiscard]] int                       WindowHeight() const { return mWindowHeight; }
    [[nodiscard]] int                       WindowWidth() const { return mWindowWidth; }
    Model*                                  AddModel(const std::string& modelName);
private:
    bool                                    CreateWindow();
    bool                                    CreateContext();
    bool                                    InitAPI();
    void                                    DestroyWindow();
    void                                    KillAPI();

    int                                     mWindowHeight;
    int                                     mWindowWidth;
    SDL_Window*                             mWindow;
    SDL_GLContext                           mContext;
    std::vector<Model>                      mModels;
    std::map<std::string, ShaderProgram>    mShaders;
};


#endif //ZZRENDERER_RENDERSYSTEM_HPP
