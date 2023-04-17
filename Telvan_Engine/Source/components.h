#pragma once

#ifndef COMPONENT_CLASS_H
#define COMPONENT_CLASS_H

//#include "entity.h"
#include "serialize.h"

class Entity;

enum class Component_Type
{
    ct_Transform = 0,
    ct_Sprite_Renderer,
    ct_Input_Controller,
    ct_Camera,
    ct_Collider
};

class Component
{
protected:
    Entity* parent_;
    Component_Type type_;

public:
    Component(Component_Type type) : parent_(nullptr), type_(type) {}

    virtual Component* Clone() { return nullptr; }

    inline void Set_Parent(Entity* parent) { parent_ = parent; }
    inline Entity* Get_Parent() const { return parent_; }

    inline virtual void Start() {}
    inline virtual void Pre_Update(float dT) {}
    inline virtual void Update(float dT) {}
    inline virtual void Post_Update(float dT) {}
    inline virtual void Render() {}
    inline virtual void Stop() {}
    inline virtual void Delete() {}

    inline virtual void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) {}
    inline virtual void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer) {}

    inline Component_Type Get_Type() { return type_; }
};

#endif
