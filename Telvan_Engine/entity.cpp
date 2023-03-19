#include "entity.h"
#include "transform.h"

// Base Functions
Entity::Entity(std::string name) : name_(name),
destroy_(false),
instantiated_(false),
parent_(nullptr)
{
    Add_Component<Transform>();
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
    {
        components_[i]->Pre_Update(dT);
    }
}

void Entity::Update(float dT)
{
    for (unsigned i = 0; i < components_.size(); i++)
    {
        components_[i]->Update(dT);
    }
}

void Entity::Post_Update(float dT)
{
    for (unsigned i = 0; i < components_.size(); i++)
    {
        components_[i]->Post_Update(dT);
    }
}

void Entity::Render()
{
    for (unsigned i = 0; i < components_.size(); i++)
    {
        components_[i]->Render();
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
template <class T>
T* Entity::Add_Component()
{
    Component* component = (Component*)Get_Component<T>();

    if (component == nullptr)
    {
        component = (Component*)new T();

        component->Set_Parent(this);

        components_.push_back(component);

        if (instantiated_)
            component->Start();
    }

    return (T*)component;
}

template <class T>
T* Entity::Get_Component() const
{
    for (int i = 0; i < components_.size(); i++)
    {
        if ((T*)components_[i] != nullptr) return (T*)components_[i];
    }

    return nullptr;
}

template <class T>
void Entity::Remove_Component(T* component)
{
    auto result = std::find(components_.begin(), components_.end(), component);

    if (result == components_.end()) return;

    int index = result - components_.begin();

    (*result)->Stop();
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
