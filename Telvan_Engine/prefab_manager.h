#pragma once

#ifndef PREFAB_MANAGER_CLASS_H
#define PREFAB_MANAGER_CLASS_H

#include <map>
#include <string>
#include <filesystem>

#include "entity.h"
#include "resource_manager.h"

class Prefab_Manager : public Resource_Manager<Entity>
{
protected:
    Prefab_Manager() {}
    static Prefab_Manager* instance_;

public:
    inline static Prefab_Manager* Get_Instance() 
    {
        if (instance_ == nullptr) instance_ = new Prefab_Manager();
        return instance_;
    }

    void Clear() override;
};

#endif // !PREFAB_MANAGER_CLASS_H

