#ifndef ERROR_LOGGING_CLASS_H
#define ERROR_LOGGING_CLASS_H

#include <string>
#include <fstream>

class Error_Logging
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
    void Output_To_Console(const std::string& message,
        Message_Level type,
        std::string class_nume = "",
        std::string function_name = "");

public:
    Error_Logging() : name_("Error Logging"), level_(Message_Level::ot_Information) {}
    static Error_Logging* Get_Instance();
    ~Error_Logging() {}

    void Initialize();
    void Shutdown();

    inline void Set_Message_Level(Message_Level level) { level_ = level; }

    static std::string Format_Output(std::string format, ...);
    void Record_Message(const std::string& message, 
        Message_Level type,
        std::string system_name = "",
        std::string function_name = "");

    inline const std::string Name() const { return name_; }
};

#endif // !ERROR_LOGGING_CLASS_H

