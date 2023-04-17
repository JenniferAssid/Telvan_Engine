/*************************************************************************************************************************************
* Title: texture_manager.h
* Author: Jennifer Assid
* Date of Creation: 20 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef TEXTURE_MANAGER_CLASS_H
#define TEXTURE_MANAGER_CLASS_H

#include <map>
#include <string>

#include "resource_manager.h"

class Texture;

class Texture_Manager : public Resource_Manager<Texture>
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
    static Texture_Manager* instance_;

protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
    Texture_Manager() : Resource_Manager<Texture>("Texture_Manager", "./Assets/Textures/") {}

protected:
public:
    static inline Texture_Manager* Get_Instance()
    {
        if (instance_ == nullptr)
        {
            Error_Logging::Get_Instance()->Record_Message(
                "Instance Created",
                Error_Logging::Message_Level::ot_Information,
                "Texture_Manager",
                "Get_Instance"
            );
            instance_ = new Texture_Manager();
        }

        return instance_;
    }

    void Clear() override;
};

#endif // !SHADER_MANAGER_CLASS_H

