#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include <string>
#include <vector>

#include "components.h"
#include "serialize.h"

class Entity
{
private:
    std::string name_;
    std::string filepath_;
    bool destroy_;
    bool instantiated_;
    std::vector<Component*> components_;

    Entity* parent_;
    std::vector<Entity*> children_;

public:
    // Base Functions
    Entity(std::string name);
    // TODO: Make a copy constructor
    ~Entity();

    // TODO: Serialize/Deserialize entity

    // Logic Functions
    void Start();
    void Pre_Update(float dT);
    void Update(float dT);
    void Post_Update(float dT);
    void Render();

    // Serialization Functions
    void Write_To();
    void Read_From();

    // General Getter Functions
    inline Entity* Get_Parent() const { return parent_; }
    inline std::string Get_Name() const { return name_; }
    inline bool Is_Destroyed() const { return destroy_; }

    // General Setter Functions
    void Set_Parent(Entity* parent);
    inline void Set_Name(std::string name) { name_ = name; }
    inline void Destroy() { destroy_ = true; }

    // Component Functions
    template <class T>
    inline T* Add_Component(Component_Type type)
    {
        Component* component = (Component*)Get_Component<T>(type);

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
    inline T* Get_Component(Component_Type type) const
    {
        for (int i = 0; i < components_.size(); i++)
        {
            T* component = (T*)components_[i];
            if (components_[i]->Get_Type() == type) return component;
        }

        return nullptr;
    }

    template <class T>
    inline void Remove_Component(T* component)
    {
        auto result = std::find(components_.begin(), components_.end(), component);

        if (result == components_.end()) return;

        int index = result - components_.begin();

        (*result)->Stop();

        components_.erase(components_.begin() + index);
    }

    inline std::vector<Component*>& Get_Components() { return components_; }

    // Children Functions
    void Add_Child(Entity* child);
    void Remove_Child(Entity* child);
    Entity* Find_Child(Entity* child);

    inline std::vector<Entity*>& Get_Children() { return children_; }
};

//#include "entity.cpp"
#endif
