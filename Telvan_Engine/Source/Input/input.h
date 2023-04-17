/*************************************************************************************************************************************
* Title: input.h
* Author: Jennifer Assid
* Date of Creation: 13 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <vector>

class Input_Controller;

class Input
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:
    using Callback = std::function<void(float, Input_Controller&)>;

    enum class Callback_Type
    {
        cb_Press = 0,
        cb_Down,
        cb_Release
    };

    struct Input_Information
    {
        bool next;
        bool current;

        Callback on_press;
        Callback on_down;
        Callback on_release;
    };

/************************************************************ VARIABLES **************************************************************/
private:
    static Input* instance_;

    std::map<int, Input_Information> bindings_;

    std::vector<int> events_;

protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
    Input() {}

    bool is_active_key(int key) { return bindings_.count(key); }

protected:
public:
    Input(const Input& other) = delete;
    static Input* Get_Instance();

    void Add_Binding(int key, const Callback& function, Callback_Type type);
    void Remove_Binding(int key, Callback_Type type);
    bool Is_Binded(int key, Callback_Type type);

    void Internal_Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    inline std::vector<int>& Get_Events() { return events_; }
    Input_Information Get_Info_From_Key(int key) { if (is_active_key(key)) return bindings_[key]; }


    inline void Set_Current_State_To_Next_Of_Key(int key) { if (is_active_key(key)) bindings_[key].current = bindings_[key].next; }
};

void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
