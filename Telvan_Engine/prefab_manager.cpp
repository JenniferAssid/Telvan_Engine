#include "prefab_manager.h"

Prefab_Manager* Prefab_Manager::instance_;
std::map<std::string, Entity*> Prefab_Manager::prefabs_;

void Prefab_Manager::open_files(std::string path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory() == true) open_files(entry.path().string() + "/");
        else
        {
            Entity* tmp = new Entity(entry.path());
            prefabs_[entry.path().stem().string()] = tmp;
        }
    }
}

Prefab_Manager* Prefab_Manager::Get_Instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Prefab_Manager();
    }
    return instance_;
}

void Prefab_Manager::Initialize()
{
    std::string path = "Data/Prefabs/";
    open_files(path);
}

Entity* Prefab_Manager::Get_Prefab(std::string name)
{
    return prefabs_[name];
}

void Prefab_Manager::Clear()
{
    for (auto iter : prefabs_)
    {
        while (iter.second->Get_Components().empty() == false)
        {
            iter.second->Get_Components().pop_back();
        }
    }
}
