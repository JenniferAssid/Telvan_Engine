/*************************************************************************************************************************************
* Title: shader_manager.h
* Author: Jennifer Assid
* Date of Creation: 20 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef SHADER_MANAGER_CLASS_H
#define SHADER_MANAGER_CLASS_H

#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include "resource_manager.h"

class Shader;

class Shader_Manager : public Resource_Manager<Shader>
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
protected:
    static Shader_Manager* instance_;

    std::vector<std::filesystem::path> vertexs_;
    std::vector<std::filesystem::path> fragments_;

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
    Shader_Manager() : Resource_Manager<Shader>("Shader_Manager", "./Assets/Shaders/") {}
    Shader* load_shader_from_file(const char* s_vertex,
        const char* s_fragment,
        const char* s_geometry = nullptr);
    void open_files(std::string path) override;

public:
    static inline Shader_Manager* Get_Instance()
    {
        if (instance_ == nullptr)
        {
                Error_Logging::Get_Instance()->Record_Message(
                    "Instance Created",
                    Error_Logging::Message_Level::ot_Information,
                    "Shader_Manager",
                    "Get_Instance"
                );
            instance_ = new Shader_Manager();
        }

        return instance_;
    }

    void Initialize() override;

    void Clear() override;
};

#endif // !SHADER_MANAGER_CLASS_H

