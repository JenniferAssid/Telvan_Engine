#pragma once

#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <vector>

class Input
{
    using Callback = std::function<void(float)>;

    struct Input_Information
    {
        bool next;
        bool current;

        Callback on_press;
        Callback on_down;
        Callback on_release;
    };

private:
    static Input* instance_;

    std::map<int, Input_Information> bindings_;

    std::vector<int> events_;
private:
    Input() {}

public:
    enum class Callback_Type
    {
        cb_Press = 0,
        cb_Down,
        cb_Release
    };

    Input(const Input& other) = delete;
    static Input* Get_Instance();

    void Handle_Events(float dT);

    void Add_Binding(int key, const Callback& function, Callback_Type type);
    bool Is_Binded(int key, Callback_Type type);

    void Internal_Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
