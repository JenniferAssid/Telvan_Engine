#include "entity.h"
#include "transform.h"
#include "sprite_renderer.h"
#include "input_controller.h"
#include "error_logging.h"

#include "prefab_manager.h"

void Entity::overwrite_prefab(bool preserve_transform)
{
    System* prefab_manager = (System*)Prefab_Manager::Get_Instance();
    if (prefab_ == "")
        prefab_ = prefab_manager->Get_Master_Path() + name_ + ".json";

    unsigned counter = 0;

    while (std::filesystem::exists(prefab_))
    {
        prefab_ = prefab_manager->Get_Master_Path() + name_ + "_" + std::to_string(counter) + ".json";
        counter++;
    }

    std::ofstream ofs(prefab_.c_str());

    if (ofs.is_open() == false)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Failed to create file \"%s\"",
                prefab_.c_str()),
            Error_Logging::Message_Level::ot_Error,
            "Entity",
            "Write_To");
        return;
    }

    Error_Logging::Get_Instance()->Record_Message(
        Error_Logging::Get_Instance()->Format_Output("Created file \"%s\"",
            prefab_.c_str()),
        Error_Logging::Message_Level::ot_Information,
        "Entity",
        "Write_To");

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    write_out_instance(writer, preserve_transform);

    ofs.clear();
    ofs << sb.GetString();
    ofs.close();
}

void Entity::write_out_instance(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
    bool preserve_transform)
{
    writer.StartObject();

    writer.Key("name");
    writer.String(name_.c_str());

    for (unsigned i = 0; i < components_.size(); i++)
        components_[i]->Write_To(writer);

    if (children_.empty() == true)
    {
        writer.EndObject();
        return;
    }

    writer.Key("children");
    writer.StartArray();

    for (unsigned i = 0; i < children_.size(); i++)
    {
        if (children_[i]->prefab_ != "")
        {
            children_[i]->write_out_prefab(writer, preserve_transform);
        }
        else
            children_[i]->write_out_instance(writer, preserve_transform);
    }

    writer.EndArray();
    writer.EndObject();
}

void Entity::write_out_prefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
    bool preserve_transform)
{
    writer.StartObject();

    writer.Key("name");
    writer.String(name_.c_str());

    writer.Key("prefab");
    writer.String(prefab_.c_str());

    Transform* transform = Get_Component<Transform>(Component_Type::ct_Transform);
    if (transform != nullptr)
        transform->Write_To(writer, false);

    if (children_.empty() == true)
    {
        writer.EndObject();
        return;
    }

    writer.Key("children");
    writer.StartArray();

    for (unsigned i = 0; i < children_.size(); i++)
    {
        if (children_[i]->prefab_ != "")
        {
            children_[i]->write_out_prefab(writer, preserve_transform);
        }
        else
            children_[i]->write_out_instance(writer, preserve_transform);
    }

    writer.EndArray();
    writer.EndObject();
}

void Entity::read_from_object(rapidjson::GenericObject<false, rapidjson::Value>& reader)
{
    if (reader.HasMember("name") == false) name_ = "No_Name";
    else name_ = reader["name"].GetString();

    if (reader.HasMember("transform"))
    {
        Add_Component<Transform>(Component_Type::ct_Transform)->Read_From(reader);
    }
    if (reader.HasMember("sprite_renderer"))
    {
        Add_Component<Sprite_Renderer>(Component_Type::ct_Sprite_Renderer)->Read_From(reader);
    }
    if (reader.HasMember("input_controller"))
    {
        Add_Component<Input_Controller>(Component_Type::ct_Input_Controller)->Read_From(reader);
    }

    if (reader.HasMember("children") &&
        reader["children"].IsArray())
    {
        rapidjson::Value::ValueIterator itr;

        if (reader["children"].Begin() == nullptr)
        {
            return;
        }

        for (itr = reader["children"].Begin();
            itr != reader["children"].End();
            itr++)
        {
            rapidjson::GenericObject<false, rapidjson::Value> child_reader = itr->GetObject();

            if (child_reader.HasMember("prefab"))
            {
                std::string entity_prefab = child_reader["prefab"].GetString();
                std::string entity_name = entity_prefab;

                if (child_reader.HasMember("name"))
                    entity_name = child_reader["name"].GetString();

                Entity* child_entity = new Entity(entity_name, entity_prefab);
                Transform* child_transform = child_entity->Add_Component<Transform>(Component_Type::ct_Transform);

                if (child_transform != nullptr && child_reader.HasMember("transform"))
                    child_transform->Read_From(child_reader);

                Add_Child(child_entity);
            }
            else
            {
                Entity* child_entity = new Entity();
                child_entity->read_from_object(child_reader);

                Add_Child(child_entity);
            }
        }
    }
}

// Base Functions
Entity::Entity(std::string name) : name_(name),
destroy_(false),
instantiated_(false),
parent_(nullptr)
{
    prefab_ = "";

    Add_Component<Transform>(Component_Type::ct_Transform);
}

Entity::Entity(std::filesystem::path path) : destroy_(false),
instantiated_(false),
parent_(nullptr)
{
    name_ = prefab_ = path.stem().string();
    Read_From(path);
}

Entity::Entity(const Entity& other)
{
    name_ = other.name_;
    instantiated_ = false;
    parent_ = nullptr;
    destroy_ = false;
    prefab_ = other.prefab_;

    for (Component* component : other.components_)
    {
        Component* clone = component->Clone();
        clone->Set_Parent(this);
        components_.push_back(clone);
        if (instantiated_)
            clone->Start();
    }

    for (Entity* entity : other.children_)
    {
        Add_Child(new Entity(*entity));
    }
}

Entity::~Entity()
{
    for (unsigned i = 0; i < components_.size(); i++)
    {
        components_[i]->Stop();
        delete components_[i];
    }

    components_.clear();
}

Entity::Entity(std::string name, std::string prefab) : destroy_(false),
instantiated_(false),
parent_(nullptr)
{
    name_ = name;
    prefab_ = prefab;
}

// Logic Functions
void Entity::Start()
{
    if (instantiated_) return;

    instantiated_ = true;

    for (unsigned i = 0; i < components_.size(); i++)
    {
        components_[i]->Start();
    }
}

void Entity::Pre_Update(float dT)
{
    for (unsigned i = 0; i < components_.size(); i++)
        components_[i]->Pre_Update(dT);

    for (unsigned i = 0; i < children_.size(); i++)
        children_[i]->Pre_Update(dT);
}

void Entity::Update(float dT)
{
    for (unsigned i = 0; i < components_.size(); i++)
        components_[i]->Update(dT);

    for (unsigned i = 0; i < children_.size(); i++)
        children_[i]->Update(dT);
}

void Entity::Post_Update(float dT)
{
    for (unsigned i = 0; i < components_.size(); i++)
        components_[i]->Post_Update(dT);

    for (unsigned i = 0; i < children_.size(); i++)
        children_[i]->Post_Update(dT);
}

void Entity::Render()
{
    for (unsigned i = 0; i < components_.size(); i++)
    {
        if (components_[i]->Get_Type() == Component_Type::ct_Collider) continue;

        components_[i]->Render();
    }

    for (unsigned i = 0; i < children_.size(); i++)
        children_[i]->Render();
}

// Serialization Functions
void Entity::Write_To(bool overwrite,
    bool preserve_transform,
    rapidjson::PrettyWriter<rapidjson::StringBuffer>* scene)
{
    if (scene != nullptr)
    {
        if (prefab_ != "")
            write_out_prefab(*scene, preserve_transform);
        else
            write_out_instance(*scene, preserve_transform);
        return;
    }

    if (overwrite == false)
        prefab_ = "";

    overwrite_prefab(preserve_transform);
}

void Entity::Read_From(std::filesystem::path path)
{
    if (std::filesystem::exists(path) == false) return;

    Serialize* serialize = new Serialize(path.string().c_str());

    rapidjson::GenericObject<false, rapidjson::Value> entity = serialize->document_.GetObject();

    read_from_object(entity);

    delete serialize;
}

// General Setter Functions
void Entity::Set_Parent(Entity* parent)
{
    if (parent_ == parent) return;

    if (parent_ != nullptr)
    {
        parent_->Remove_Child(this);
        parent_ = nullptr;
    }

    parent->Add_Child(this);
    parent_ = parent;
}

// Children Functions
void Entity::Add_Child(Entity* child)
{
    if (child == nullptr) return;

    Entity* tmp_child = Find_Child(child);

    if (tmp_child == child) return;
    if (this == child->parent_) return;

    if (child->parent_ != nullptr)
    {
        child->parent_->Remove_Child(child);
    }

    child->parent_ = this;
    children_.push_back(child);
}

void Entity::Remove_Child(Entity* child)
{
    if (child == nullptr) return;

    Entity* tmp_child = Find_Child(child);

    if (tmp_child == nullptr) return;

    child->parent_ = nullptr;

    auto placement = std::find(children_.begin(), children_.end(), child);
    if (placement == children_.end()) return;

    int index = placement - children_.begin();

    children_.erase(children_.begin() + index);
}

Entity* Entity::Find_Child(Entity* child)
{
    if (child == nullptr) return nullptr;

    auto placement = std::find(children_.begin(), children_.end(), child);
    if (placement == children_.end()) return nullptr;

    int index = placement - children_.begin();

    return children_.at(index);
}
