#pragma once

#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <map>
#include <string>
#include <filesystem>

#include "system.h"

template <class Resource>
class Resource_Manager : public System
{
protected:
    std::map<std::string, Resource*> resources_;

    void virtual open_files(std::string path)
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory() == true) open_files(entry.path().string() + "/");
            else
            {
                Resource* tmp = new Resource(entry.path());
                resources_[entry.path().stem().string()] = tmp;
            }
        }
    }

public:
    Resource_Manager(std::string name = "Resource_Manager-No_Name",
        std::string master_path = "") : System(name, master_path) {}

    void virtual Initialize() override
    {
        if (std::filesystem::exists(master_path_) == false) return;
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
