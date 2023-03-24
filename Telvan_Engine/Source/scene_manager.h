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
    Scene_Manager() : Resource_Manager<Scene>("Scene_Manager", "./Data/Scenes/") {}
    static Scene_Manager* instance_;

public:
    static inline Scene_Manager* Get_Instance()
    {
        if (instance_ == nullptr)
        {
            Error_Logging::Get_Instance()->Record_Message(
                "Instance Created",
                Error_Logging::Message_Level::ot_Information,
                "Resource_Manager",
                "Get_Instance"
            );
            instance_ = new Scene_Manager();
        }

        return instance_;
    }
};

#endif // !SCENE_MANAGER_CLASS_H

