#pragma once

#ifndef PREFAB_MANAGER_CLASS_H
#define PREFAB_MANAGER_CLASS_H

#include <map>
#include <string>
#include <filesystem>

#include "entity.h"

class Prefab_Manager
{
private:
    Prefab_Manager() {}
    static Prefab_Manager* instance_;

    static std::map<std::string, Entity*> prefabs_;

    void open_files(std::string path);

public:
    static Prefab_Manager* Get_Instance();

    void Initialize();

    static Entity* Get_Prefab(std::string name);

    static void Clear();
};

#endif // !PREFAB_MANAGER_CLASS_H

