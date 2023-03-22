#pragma once

#ifndef PREFAB_MANAGER_CLASS_H
#define PREFAB_MANAGER_CLASS_H

#include <map>
#include <string>
#include <filesystem>

#include "resource_manager.h"

class Entity;

class Prefab_Manager : public Resource_Manager<Entity>
{
protected:
    Prefab_Manager() : Resource_Manager<Entity>("Prefab_Manager", "./Data/Prefabs/") {}
    static Prefab_Manager* instance_;

public:
    inline static Prefab_Manager* Get_Instance() 
    {
        if (instance_ == nullptr) instance_ = new Prefab_Manager();
        return instance_;
    }

    void Initialize() override;

    void Clear() override;
};

#endif // !PREFAB_MANAGER_CLASS_H

