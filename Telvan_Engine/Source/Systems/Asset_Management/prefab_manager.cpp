/*************************************************************************************************************************************
* Title: prefab_manager.cpp
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
#include "prefab_manager.h"
#include "entity.h"
#include "transform.h"

Prefab_Manager* Prefab_Manager::instance_;

void Prefab_Manager::Initialize()
{
    if (std::filesystem::exists(master_path_) == false)
    {
        Error_Logging::Get_Instance()->Record_Message(
            "Master path not valid",
            Error_Logging::Message_Level::ot_Warning,
            "Prefab_Manager",
            "Initialize"
        );
    }
    open_files(master_path_);

    Error_Logging::Get_Instance()->Record_Message(
        "Attaching children objects to loaded in prefabs",
        Error_Logging::Message_Level::ot_Information,
        "Prefab_Manager",
        "Initialize"
    );
    for (auto key : resources_)
    {
        unsigned children_size = key.second->Get_Children().size();
        for (unsigned i = 0; i < children_size; i++)
        {
            Entity* entity = key.second->Get_Children().front();
            std::string prefab = entity->Get_Prefab();
            
            if (prefab != "")
            {
                std::string name = entity->Get_Name();
                Transform transform = *entity->Get_Component<Transform>(Component_Type::ct_Transform);

                key.second->Remove_Child(entity);
                delete entity;

                entity = new Entity(*Get_Resource(prefab));
                entity->Set_Name(name);
                entity->Set_Prefab(prefab);

                Transform* entity_transform = entity->Get_Component<Transform>(Component_Type::ct_Transform);
                entity_transform->Set_Translation(transform.Get_Translation());

                if (entity_transform->Get_Scale() == glm::vec2(-1.0f, -1.0f))
                {
                    entity_transform->Set_Scale(transform.Get_Scale());
                    entity_transform->Set_Rotation(transform.Get_Rotation());
                }
            }

            key.second->Add_Child(entity);
        }
    }
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
