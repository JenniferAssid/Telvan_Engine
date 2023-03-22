#include "entity.h"
#include "transform.h"
#include "sprite_renderer.h"
#include "input_controller.h"
#include "error_logging.h"

// Base Functions
Entity::Entity(std::string name) : name_(name),
destroy_(false),
instantiated_(false),
parent_(nullptr)
{
    filepath_ = "./Data/Prefabs" + name + ".json";

    Add_Component<Transform>(Component_Type::ct_Transform);
}

Entity::Entity(std::filesystem::path path) : destroy_(false),
instantiated_(false),
parent_(nullptr)
{
    filepath_ = path.string();
    Read_From(filepath_);
}

Entity::Entity(const Entity& other)
{
    name_ = other.name_;
    instantiated_ = false;
    parent_ = nullptr;
    destroy_ = false;
    filepath_ = other.filepath_;

    for (Component* component : other.components_)
    {
        Component* clone = component->Clone();
        clone->Set_Parent(this);
        components_.push_back(clone);
        if (instantiated_)
            clone->Start();
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
        components_[i]->Render();

    for (unsigned i = 0; i < children_.size(); i++)
        children_[i]->Render();
}

// Serialization Functions
void Entity::Write_To(bool overwrite_prefab,
    rapidjson::PrettyWriter<rapidjson::StringBuffer>* scene)
{
    if (scene != nullptr)
    {
        scene->StartObject();

        scene->Key("name");
        scene->String(name_.c_str());

        scene->Key("prefab");
        scene->String(filepath_.c_str());

        Transform* transform = Get_Component<Transform>(Component_Type::ct_Transform);
        if (transform != nullptr)
        {
            transform->Set_Print_Full_Transform(true);
            transform->Write_To(*scene);
        }

        scene->EndObject();
        return;
    }

    unsigned int counter = 0;

    while (std::filesystem::exists(filepath_) && overwrite_prefab == false)
    {
        filepath_ = "./Data/Prefabs/" + name_ + "_" + std::to_string(counter) + ".json";
    }

    std::ofstream ofs(filepath_.c_str());

    if (ofs.is_open() == false)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Failed to create file \"%s\"",
                filepath_.c_str()),
            Error_Logging::Message_Level::ot_Error,
            "Entity",
            "Write_To");
        return;
    }

    Error_Logging::Get_Instance()->Record_Message(
        Error_Logging::Get_Instance()->Format_Output("Created file \"%s\"",
            filepath_.c_str()),
        Error_Logging::Message_Level::ot_Information,
        "Entity",
        "Write_To");

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());

    for (unsigned i = 0; i < components_.size(); i++)
    {
        components_[i]->Write_To(writer);
    }

    writer.EndObject();

    ofs.clear();
    ofs << sb.GetString();
    ofs.close();
}

void Entity::Read_From(std::string filepath)
{
    std::string file = (filepath == "") ? filepath_ : filepath;

    Serialize* serialize = new Serialize(file);

    if (serialize->document_.HasMember("name") == false) name_ = "No_Name";
    else name_ = serialize->document_["name"].GetString();

    rapidjson::GenericObject<false, rapidjson::Value> writer = serialize->document_.GetObject();

    if (serialize->document_.HasMember("transform"))
    {
        Add_Component<Transform>(Component_Type::ct_Transform)->Read_From(writer);
    }
    if (serialize->document_.HasMember("sprite_renderer"))
    {
        Add_Component<Sprite_Renderer>(Component_Type::ct_Sprite_Renderer)->Read_From(writer);
    }
    if (serialize->document_.HasMember("input_controller"))
    {
        Add_Component<Input_Controller>(Component_Type::ct_Input_Controller)->Read_From(writer);
    }
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

// Component Functions
//template <class T>
//T* Entity::Add_Component()
//{
//    Component* component = (Component*)Get_Component<T>();
//
//    if (component == nullptr)
//    {
//        component = (Component*)new T();
//
//        component->Set_Parent(this);
//
//        components_.push_back(component);
//
//        if (instantiated_)
//            component->Start();
//    }
//
//    return (T*)component;
//}

//template <class T>
//T* Entity::Get_Component() const
//{
//    for (int i = 0; i < components_.size(); i++)
//    {
//        if ((T*)components_[i] != nullptr) return (T*)components_[i];
//    }
//
//    return nullptr;
//}

//template <class T>
//void Entity::Remove_Component(T* component)
//{
//    auto result = std::find(components_.begin(), components_.end(), component);
//
//    if (result == components_.end()) return;
//
//    int index = result - components_.begin();
//
//    (*result)->Stop();
//}

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
