//
// Created by lorenzo on 7/5/20.
//

#ifndef ZZRENDERER_RENDERCOMPONENT_HPP
#define ZZRENDERER_RENDERCOMPONENT_HPP

#include <memory>
#include "Model.hpp"

class RenderComponent
{
public:
    RenderComponent(std::string name, std::shared_ptr<Model> m)
    : mName(name)
    , mModel(m)
    {}
    ~RenderComponent(){}

    std::string mName;
    std::shared_ptr<Model> mModel;
};
#endif //ZZRENDERER_RENDERCOMPONENT_HPP
