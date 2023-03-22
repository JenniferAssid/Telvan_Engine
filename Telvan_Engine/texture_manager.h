#pragma once

#ifndef TEXTURE_MANAGER_CLASS_H
#define TEXTURE_MANAGER_CLASS_H

#include <map>
#include <string>

#include "texture.h"
#include "resource_manager.h"

class Texture_Manager : public Resource_Manager<Texture>
{
private:
    Texture_Manager() : Resource_Manager<Texture>("Texture_Manager", "./Assets/Textures/") {}
    static Texture_Manager* instance_;

public:
    static inline Texture_Manager* Get_Instance()
    {
        if (instance_ == nullptr)
            instance_ = new Texture_Manager();

        return instance_;
    }

    void Clear() override;
};

#endif // !SHADER_MANAGER_CLASS_H

