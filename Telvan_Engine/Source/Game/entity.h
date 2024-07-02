/*************************************************************************************************************************************
* Title: entity.h
* Author: Jennifer Assid
* Date of Creation: 13 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include <string>
#include <vector>
#include <rapidjson/prettywriter.h>
#include <filesystem>

#include "engine.h"
#include "components.h"
#include "serialize.h"
#include "collider_manager.h"
#include "sprite_renderer_manager.h"

class Entity
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
    std::string name_;
    std::string prefab_;
    bool destroy_;
    bool instantiated_;
    std::vector<Component*> components_;

    Entity* parent_;
    std::vector<Entity*> children_;

protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
    void overwrite_prefab(bool preserve_transform);
    void write_out_instance(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
        bool preserve_transform);
    void write_out_prefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
        bool preserve_transform);

    void read_from_object(rapidjson::GenericObject<false, rapidjson::Value>& reader);

protected:
public:
    // Base Functions
    Entity() : name_("NoName") {}
    Entity(std::string name);
    Entity(std::filesystem::path path);
    Entity(const Entity& other);
    Entity(std::string name, std::string prefab);
    ~Entity();

    // Logic Functions
    void Start();
    void Pre_Update(float dT);
    void Update(float dT);
    void Post_Update(float dT);
    void Render();

    // Write To Functions
    void Write_To(bool overwrite = false,
        bool preserve_transform = false,
        rapidjson::PrettyWriter<rapidjson::StringBuffer>* scene = nullptr);

    // Read From Functions
    void Read_From(std::filesystem::path path);


    // General Getter Functions
    inline Entity* Get_Parent() const { return parent_; }
    inline std::string Get_Name() const { return name_; }
    inline bool Is_Destroyed() const { return destroy_; }
    inline std::string Get_Prefab() const { return prefab_; }

    // General Setter Functions
    void Set_Parent(Entity* parent);
    inline void Set_Name(std::string name) { name_ = name; }
    inline void Destroy() { destroy_ = true; }
    inline void Set_Prefab(std::string prefab) { prefab_ = prefab; }

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

    inline void Remove_Component(Component_Type type)
    {
        int index = 0;
        for (index; index < components_.size(); index++)
        {
            if (components_[index]->Get_Type() == type)
                break;
        }

        if (index >= components_.size()) return;

        if (components_[index]->Get_Type() == Component_Type::ct_Collider)
            Collider_Manager::Get_Instance()->Remove_Collider((Collider*)components_[index]);
        if (components_[index]->Get_Type() == Component_Type::ct_Sprite_Renderer)
            Sprite_Renderer_Manager::Get_Instance()->Remove_Sprite_Renderer((Sprite_Renderer*)components_[index]);

        components_[index]->Stop();

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
