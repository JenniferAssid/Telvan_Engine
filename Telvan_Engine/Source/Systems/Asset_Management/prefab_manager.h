/*************************************************************************************************************************************
* Title: prefab_manager.h
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
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
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
protected:
    static Prefab_Manager* instance_;

public:

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
    Prefab_Manager() : Resource_Manager<Entity>("Prefab_Manager", "./Data/Prefabs/") {}
public:
    inline static Prefab_Manager* Get_Instance() 
    {
        if (instance_ == nullptr)
        {
            Error_Logging::Get_Instance()->Record_Message(
                "Instance Created",
                Error_Logging::Message_Level::ot_Information,
                "Error_Logging",
                "Get_Instance"
            );
            instance_ = new Prefab_Manager();
        }
        return instance_;
    }

    void Initialize() override;

    void Clear() override;
};

#endif // !PREFAB_MANAGER_CLASS_H

