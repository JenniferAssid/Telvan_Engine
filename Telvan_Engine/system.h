#pragma once

#ifndef SYSTEM_CLASS_H
#define SYSTEM_CLASS_H

#include <string>

class System
{
protected:
    std::string master_path_;
    std::string name_;

public:
    System(std::string name, std::string master_path) : name_(name), master_path_(master_path) {}

    void virtual Initialize() {}
    void virtual Shutdown() {}

    inline std::string Get_Name() { return name_; }
    inline std::string Get_Master_Path() { return master_path_; }
};

#endif // !SYSTEM_CLASS_H

