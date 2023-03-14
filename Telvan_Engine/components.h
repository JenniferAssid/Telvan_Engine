#ifndef COMPONENT_CLASS_H
#define COMPONENT_CLASS_H

#include "entity.h"

class Component
{
private:
    Entity* parent_;

public:
    Component() : parent_(nullptr) {}

    inline void SetParent(Entity* parent) { parent_ = parent; }
    inline virtual void Start() {}
    inline virtual void Update(float dT) {}
    inline virtual void Stop() {}
    inline virtual void Delete() {}
};

#endif
