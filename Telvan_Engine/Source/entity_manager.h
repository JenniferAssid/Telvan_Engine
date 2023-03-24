#pragma once

#ifndef ENTITY_MANAGER_CLASS_H
#define ENTITY_MANAGER_CLASS_H

#include <vector>

class Entity;

class Entity_Manager
{
private:
    std::vector<Entity*> active_entity_;
    Entity_Manager() {}
    static Entity_Manager* instance_;

public:
    static Entity_Manager* Get_Instance();

    void Start();
    
    void Update(float dT);
    void Render();

    void Add_Entity(Entity* entity);

    void Clear();
    void Shutdown();
};

#endif // !ENTITY_MANAGER_CLASS_H

