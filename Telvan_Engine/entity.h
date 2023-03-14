#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include <string>
#include <vector>


class Entity
{
private:
    std::string name_;
    //std::vector<Component*> components_;

    bool destroy_;

public:
    inline Entity(std::string name) : name_(name) {}

    /*void AddComponent(Component* component)
    {
        component->SetParent(this);
        component->Start();

        components_.push_back(component);
    }*/

    /*void Update(float dT)
    {
        for (int i = 0; i < components_.size(); i++)
        {
            components_[i]->Update(dT);
        }
    }

    inline void SetToDestroy() { destroy_ = true; }

    ~Entity()
    {
        for (int i = 0; i < components_.size(); i++)
        {
            components_[i]->Delete();
            components_[i] = nullptr;
        }

        components_.clear();
    }*/
};

#endif
