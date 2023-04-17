/*************************************************************************************************************************************
* Title: system.h
* Author: Jennifer Assid
* Date of Creation: 21 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef SYSTEM_CLASS_H
#define SYSTEM_CLASS_H

#include <string>

class Engine;

class System
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
protected:
    std::string master_path_;
    std::string name_;

    Engine* engine_;

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
    System(std::string name, std::string master_path) : 
        name_(name), master_path_(master_path)
    {}

    void virtual Initialize() {}
    void virtual Shutdown() {}

    inline void Set_Engine(Engine* engine) { engine_ = engine; }

    inline std::string Get_Name() { return name_; }
    inline std::string Get_Master_Path() { return master_path_; }
    inline Engine* Get_Engine() { return engine_; }
};

#endif // !SYSTEM_CLASS_H

