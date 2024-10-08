#include "input.h"
#include "error_logging.h"

Input* Input::instance_ = nullptr;

Input* Input::Get_Instance()
{
    if (instance_ == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            "Instance Created",
            Error_Logging::Message_Level::ot_Information,
            "Input",
            "Get_Instance");
        instance_ = new Input();
    }
    return instance_;
}

void Input::Add_Binding(int key,
    const Callback& function,
    Callback_Type type)
{
    switch (type)
    {
    case Input::Callback_Type::cb_Press:
        bindings_[key].on_press = function;
        break;
    case Input::Callback_Type::cb_Down:
        bindings_[key].on_down = function;
        break;
    case Input::Callback_Type::cb_Release:
        bindings_[key].on_release = function;
        break;
    default:
        break;
    }
}

void Input::Remove_Binding(int key,
    Callback_Type type)
{
    switch (type)
    {
    case Input::Callback_Type::cb_Press:
        bindings_[key].on_press = nullptr;
        break;
    case Input::Callback_Type::cb_Down:
        bindings_[key].on_down = nullptr;
        break;
    case Input::Callback_Type::cb_Release:
        bindings_[key].on_release = nullptr;
        break;
    default:
        break;
    }
}

bool Input::Is_Binded(int key,
    Callback_Type type)
{
    if (bindings_.find(key) == bindings_.end()) return false;

    switch (type)
    {
    case Input::Callback_Type::cb_Press:
        if (bindings_[key].on_press == nullptr) return false;
        break;
    case Input::Callback_Type::cb_Down:
        if (bindings_[key].on_down == nullptr) return false;
        break;
    case Input::Callback_Type::cb_Release:
        if (bindings_[key].on_release == nullptr) return false;
        break;
    default:
        break;
    }

    return true;
}

void Input::Internal_Keyboard_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    Input* input = Input::Get_Instance();

    if (input->bindings_.find(key) == bindings_.end()) 
        return;

    if (action == GLFW_PRESS)
    {
        bindings_[key].next = true;
        events_.push_back(key);
    }
    else if (action == GLFW_RELEASE)
    {
        bindings_[key].next = false;


        auto var = std::find(events_.begin(), events_.end(), key);
        while (var != events_.end())
        {
            events_.erase(events_.begin() + (var - events_.begin()));
            var = std::find(events_.begin(), events_.end(), key);
        }

        events_.push_back(key);
    }
}

void Keyboard_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    Input* input = Input::Get_Instance();

    input->Internal_Keyboard_Callback(window, key, scancode, action, mods);
}
