/*************************************************************************************************************************************
* Title: scene.h
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include <string>
#include <filesystem>

class Scene
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
    std::string name_;
    
protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
    Scene(std::filesystem::path path) : name_(path.stem().string()) {}
    void Load();
    void Save() {}

    inline std::string Get_Name() { return name_; }
    
    inline void Set_Name(std::string name) { name_ = name; }
};

#endif // !SCENE_CLASS_H

