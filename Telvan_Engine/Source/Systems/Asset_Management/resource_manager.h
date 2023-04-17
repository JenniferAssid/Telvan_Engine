/*************************************************************************************************************************************
* Title: resource_manager.h
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <map>
#include <string>
#include <filesystem>

#include "system.h"
#include "error_logging.h"

template <class Resource>
class Resource_Manager : public System
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
protected:
    std::map<std::string, Resource*> resources_;

public:

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
    void virtual open_files(std::string path)
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory() == true) open_files(entry.path().string() + "/");
            else
            {
                Resource* tmp = new Resource(entry.path());

                if (tmp == nullptr)
                {
                    Error_Logging::Get_Instance()->Record_Message(
                        Error_Logging::Format_Output(
                            "Failed to create asset: %s",
                            entry.path().stem().string().c_str()),
                        Error_Logging::Message_Level::ot_Error,
                        name_,
                        "open_files");
                    continue;
                }
                else
                {
                    Error_Logging::Get_Instance()->Record_Message(
                        Error_Logging::Format_Output(
                            "Successfully created asset: %s",
                            entry.path().stem().string().c_str()),
                        Error_Logging::Message_Level::ot_Information,
                        name_,
                        "open_files");
                }

                resources_[entry.path().stem().string()] = tmp;
            }
        }
    }

public:
    Resource_Manager(std::string name = "Resource_Manager-No_Name",
        std::string master_path = "") : System(name, master_path) {}

    void virtual Initialize() override
    {
        if (std::filesystem::exists(master_path_) == false)
        {
            Error_Logging::Get_Instance()->Record_Message(
                "Master path is not valid",
                Error_Logging::Message_Level::ot_Warning,
                name_,
                "Initialize"
            );
        }
        open_files(master_path_);
    }

    inline Resource* Get_Resource(std::string name)
    {
        if (resources_.count(name) == false) return nullptr;

        return resources_[name];
    }

    virtual void Clear() { resources_.clear(); }
    void inline Shutdown() override { Clear(); }
};

#endif // !RESOURCE_MANAGER_CLASS_H
