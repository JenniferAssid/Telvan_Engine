#pragma once

#ifndef SCENE_MANAGER_CLASS_H
#define SCENE_MANAGER_CLASS_H

#include <map>
#include <string>

#include "scene.h"
#include "resource_manager.h"

class Scene_Manager : public Resource_Manager<Scene>
{
private:
    Scene_Manager() {}
    static Scene_Manager* instance_;

public:
    static inline Scene_Manager* Get_Instance()
    {
        if (instance_ == nullptr)
            instance_ = new Scene_Manager();

        return instance_;
    }
};

#endif // !SCENE_MANAGER_CLASS_H

