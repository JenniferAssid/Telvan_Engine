#ifndef COMPONENT_CLASS_H
#define COMPONENT_CLASS_H

#include "entity.h"
#include "serialize.h"

class Component
{
protected:
    class Entity* parent_;

public:
    Component() : parent_(nullptr) {}

    inline void Set_Parent(Entity* parent) { parent_ = parent; }
    inline virtual void Start() {}
    inline virtual void Pre_Update(float dT) {}
    inline virtual void Update(float dT) {}
    inline virtual void Post_Update(float dT) {}
    inline virtual void Render() {}
    inline virtual void Stop() {}
    inline virtual void Delete() {}

    inline virtual void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {}
    inline virtual void Read_From(rapidjson::Document& document) {}
};

#endif
