#pragma once

#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include <string>

class Scene
{
private:
    std::string name_;

public:
    Scene(std::string name) : name_(name) {}
    void Load();
    void Save() {}

    inline std::string Get_Name() { return name_; }
    
    inline void Set_Name(std::string name) { name_ = name; }
};

#endif // !SCENE_CLASS_H

