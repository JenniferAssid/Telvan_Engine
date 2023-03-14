#ifndef ENGINE_CLASS_H
#define ENGINE_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.h"

class Engine
{
private:
    static Engine* instance_;

    unsigned int width_, height_;
    GLFWwindow* window_;

    float last_frame;
    
private:
    Engine(unsigned int width, unsigned int height);

public:
    float deltaTime;

public:
    Engine(const Engine& other) = delete;
    static Engine* Get_Instance();
    ~Engine();

    void Initialize();
    void Process_Input();
    void Update();
    void Render() {}

    inline bool Should_Engine_Shutdown() { return glfwWindowShouldClose(window_); }
};

inline void Frame_Buffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#endif
