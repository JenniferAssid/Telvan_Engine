#pragma once

#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <map>
#include <string>
#include <filesystem>

template <class Resource>
class Resource_Manager
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
    Resource_Manager() {}

    void virtual Initialize(std::string path)
    {
        open_files(path);
    }

    inline Resource* Get_Resource(std::string name)
    {
        if (resources_.count(name) == false) return nullptr;

        return resources_[name];
    }

    virtual void Clear() { resources_.clear(); }
};

#endif // !RESOURCE_MANAGER_CLASS_H
