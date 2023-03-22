#include "prefab_manager.h"
#include "entity.h"

Prefab_Manager* Prefab_Manager::instance_;

void Prefab_Manager::Initialize()
{
    if (std::filesystem::exists(master_path_) == false) return;
    open_files(master_path_);

    /*for (auto key : resources_)
    {
        for (Entity* entity : key.second->Get_Children())
        {
            std::string name = entity->Get_Name();
            delete entity;
            entity = Prefab_Manager::Get_Instance()->Get_Resource(name);
        }
    }*/
}

void Prefab_Manager::Clear()
{
    for (auto iter : resources_)
    {
        while (iter.second->Get_Components().empty() == false)
        {
            iter.second->Get_Components().pop_back();
        }
    }

    resources_.clear();
}
