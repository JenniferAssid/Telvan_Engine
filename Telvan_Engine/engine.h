#pragma once

#ifndef ENGINE_CLASS_H
#define ENGINE_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "input.h"
#include "scene.h"
#include "system.h"

class Engine
{
private:
    static Engine* instance_;

    unsigned int width_, height_;
    GLFWwindow* window_;

    float last_frame;

    Scene* current_scene_;
    std::vector<System*> systems_;
private:
    Engine(unsigned int width, unsigned int height);

public:
    float deltaTime;

public:
    Engine(const Engine& other) = delete;
    static Engine* Get_Instance();
    ~Engine();

    void Initialize();
    void Update();
    void Render();

    inline bool Should_Engine_Shutdown() { return glfwWindowShouldClose(window_); }
    inline void Register_System(System* system) { if (system != nullptr) systems_.push_back(system); }
    inline System* Find_System(std::string name) 
    { 
        std::vector<System*>::iterator itr = std::find_if(systems_.begin(), systems_.end(),
            [name](System* system)
            {
                if (system->Get_Name() == name)
                    return true;
            }
        );

        if (itr == systems_.end()) return nullptr;
        else return *itr;
    }
};

inline void Frame_Buffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#endif
