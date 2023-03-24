#include "error_logging.h"

#include <color-console/color.hpp>
#include <iostream>
#include <vector>

#include <chrono>

Error_Logging* Error_Logging::instance_ = nullptr;

void Error_Logging::Output_To_Console(const std::string& message,
    Message_Level type,
    std::string class_name,
    std::string function_name)
{
    switch (type)
    {
    case Message_Level::ot_Error:
        std::cout << "[" << dye::red("error") << "] " << dye::red(class_name);
        break;
    case Message_Level::ot_Warning:
        if (level_ == Message_Level::ot_Error) return;
        std::cout << "[" << dye::yellow("warning") << "] " << dye::yellow(class_name);
        break;
    case Message_Level::ot_Information:
        if (level_ != Message_Level::ot_Information) return;
        std::cout << "[" << dye::green("info") << "] " << dye::green(class_name);
        break;
    default:
        break;
    }

    std::cout << "::" << dye::grey(function_name) << ": " << message << std::endl;
}

Error_Logging* Error_Logging::Get_Instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Error_Logging();
        Error_Logging::Get_Instance()->Record_Message(
            "Instance Created",
            Error_Logging::Message_Level::ot_Information,
            "Error_Logging",
            "Get_Instance"
        );
    }
    return instance_;
}

void Error_Logging::Initialize()
{
    log_file_.open("./default.log");

    if (!log_file_.is_open())
    {
        Record_Message("Failed to open \"default.log\"", Message_Level::ot_Error, name_, "Initialize");
        abort();
    }
    else
    {
        Record_Message("Successfully opened \"default.log\"", Message_Level::ot_Information, name_, "Initialize");
    }

    Record_Message("Testing error print-out capabilities", Message_Level::ot_Error, name_, "Initialize");
    Record_Message("Testing warning print-out capabilities", Message_Level::ot_Warning, name_, "Initialize");
    Record_Message("Testing information print-out capabilities", Message_Level::ot_Information, name_, "Initialize");
}

void Error_Logging::Shutdown()
{
    log_file_.close();

    if (log_file_.is_open())
    {
        Record_Message("Failed to close \"default.log\"", Message_Level::ot_Error, name_, "Initialize");
    }
    else
    {
        Record_Message("Successfully closed \"default.log\"", Message_Level::ot_Information, name_, "Initialize");
    }
}

std::string Error_Logging::Format_Output(std::string format, ...)
{
    va_list args;
    va_start(args, format);
    size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
    va_end(args);
    std::vector<char> vec(len + 1);
    va_start(args, format);
    std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end(args);
    return &vec[0];
}

void Error_Logging::Record_Message(const std::string& message,
    Message_Level type,
    std::string class_name,
    std::string function_name)
{
    Output_To_Console(message, type, class_name, function_name);

    if (log_file_.is_open() == false) return;

    log_file_ << "[";

    switch (type)
    {
    case Error_Logging::Message_Level::ot_Error:
        log_file_ << "error";
        break;
    case Error_Logging::Message_Level::ot_Warning:
        log_file_ << "warning";
        break;
    case Error_Logging::Message_Level::ot_Information:
        log_file_ << "info";
        break;
    default:
        break;
    }

    log_file_ << "] " << function_name << "::" << class_name << ": " << message << std::endl;
}


