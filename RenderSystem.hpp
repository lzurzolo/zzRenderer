//
// Created by lorenzo on 5/30/20.
//

#ifndef ZZRENDERER_RENDERSYSTEM_HPP
#define ZZRENDERER_RENDERSYSTEM_HPP

#include <iostream>
#include <filesystem>
#include <map>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Model.hpp"
#include "ShaderSystem.hpp"
#include "RenderComponent.hpp"

typedef std::string ShaderName;

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
    Model&                                  AddModel(const std::string& modelName, const ShaderProgram& sp);
    void                                    RemoveModel(const std::string& modelName);
    Model&                                  GetModel(const std::string& name);
    RenderComponent&                        AddRenderComponent(std::string name, const RenderComponent& rc);
    RenderComponent&                        GetRenderComponent(std::string name);

private:
    bool                                    CreateWindow();
    bool                                    CreateContext();
    bool                                    InitAPI();
    void                                    DestroyWindow();
    void                                    KillAPI();
    void                                    ClearResources();

    int                                     mWindowHeight;
    int                                     mWindowWidth;
    SDL_Window*                             mWindow;
    SDL_GLContext                           mContext;

    std::map<std::string, RenderComponent>  mRenderComponents;

    // TODO : need a "model instance" map as there can be multiple instances of the same model obviously
    std::map<std::string, Model>            mModels;
};


#endif //ZZRENDERER_RENDERSYSTEM_HPP
