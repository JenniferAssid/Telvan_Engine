#include "scene.h"
#include "entity_manager.h"
#include "transform.h"
#include "prefab_manager.h"
#include "error_logging.h"
#include "serialize.h"

#include <filesystem>
#include <fstream>

void Scene::Load()
{
    Entity_Manager::Get_Instance()->Clear();

    std::string path = "./Data/Scenes/" + name_ + ".json";
    if (std::filesystem::exists(path) == false)
    {
        std::ofstream ofs(path);
        if (ofs.is_open() == false)
        {
            Error_Logging::Get_Instance()->Record_Message(
                Error_Logging::Get_Instance()->Format_Output("Failed to create file \"%s\"",
                    path.c_str()),
                Error_Logging::Message_Level::ot_Error,
                "Scene",
                "Load");
            return;
        }

        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Created file \"%s\"",
                path.c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Scene",
            "Load");

        ofs.close();
        return;
    }

    Serialize* serialize = new Serialize(path);

    std::string scene_name = std::filesystem::path(path).stem().string();

    name_ = scene_name;

    rapidjson::Value::ValueIterator itr;

    for (itr = itr = serialize->document_[scene_name.c_str()].Begin();
        itr != serialize->document_[scene_name.c_str()].End(); itr++)
    {
        rapidjson::GenericObject < false, rapidjson::Value> writer = itr->GetObject();

        std::string entity_name = writer["name"].GetString();
        std::filesystem::path entity_path(writer["prefab"].GetString());

        Entity* entity = new Entity(*Prefab_Manager::Get_Instance()->Get_Prefab(entity_path.stem().string()));
        entity->Set_Name(entity_name);

        Transform* transform = entity->Get_Component<Transform>(Component_Type::ct_Transform);
        
        transform->Read_From(writer);

        Entity_Manager::Get_Instance()->Add_Entity(entity);
    }
}
