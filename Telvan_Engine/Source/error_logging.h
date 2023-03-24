#pragma once

#ifndef ERROR_LOGGING_CLASS_H
#define ERROR_LOGGING_CLASS_H

#include <string>
#include <fstream>
#include <ctime>

#include "system.h"

class Error_Logging : public System
{
public:
    enum class Message_Level {
        ot_Error = 0,
        ot_Warning,
        ot_Information
    };

private:
    static Error_Logging* instance_;
    const std::string name_;
    std::ofstream log_file_;
    Message_Level level_;

private:
    Error_Logging() : System("Error_Logging", ""),
        name_("Error Logging"), level_(Message_Level::ot_Information) {}
    void Output_To_Console(const std::string& message,
        Message_Level type,
        std::string class_nume = "",
        std::string function_name = "");

public:
    
    static Error_Logging* Get_Instance();
    ~Error_Logging() {}

    void Initialize() override;
    void Shutdown() override;

    inline void Set_Message_Level(Message_Level level) { level_ = level; }

    static std::string Format_Output(std::string format, ...);
    void Record_Message(const std::string& message, 
        Message_Level type,
        std::string system_name = "",
        std::string function_name = "");

    inline const std::string Name() const { return name_; }
};

#endif // !ERROR_LOGGING_CLASS_H

