#include "prefab_manager.h"

Prefab_Manager* Prefab_Manager::instance_;

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
