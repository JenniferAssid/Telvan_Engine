#include "engine.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

#include <iostream>

Engine::Engine(unsigned int width,
    unsigned height)
{
    width_ = width;
    height_ = height;
    deltaTime = last_frame = 0.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, true);

    window_ = glfwCreateWindow(width_,
        height_,
        "Telvan's Engine",
        nullptr,
        nullptr);
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "| ERROR::Engine: Failed to initailze GLAD\n";
        abort();
    }

    glfwSetKeyCallback(window_, Keyboard_Callback);
    glfwSetFramebufferSizeCallback(window_, Frame_Buffer_Size_Callback);

    glViewport(0, 0, width_, height_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Engine* Engine::Get_Instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Engine(800, 600);
    }
    return instance_;
}

Engine::~Engine()
{
    Resource_Manager::Clear();

    glfwTerminate();
}

Sprite_Renderer* renderer;

void Engine::Initialize()
{
    deltaTime = last_frame = 0.0f;

    Resource_Manager::Load_Shader("default.vert", "default.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width_),
        static_cast<float>(this->height_), 0.0f, -1.0f, 1.0f);
    Resource_Manager::Get_Shader("sprite").Use().Set_Integer("tex", 0);
    Resource_Manager::Get_Shader("sprite").Set_Matrix_4("projection", projection);
    // set render-specific controls
    Shader tmp = Resource_Manager::Get_Shader("sprite");
    renderer = new Sprite_Renderer(tmp);
    // load textures
    Resource_Manager::Load_Texture("Assets/awesomeface.png", true, "face");
}

void Engine::Process_Input()
{
    Input::Get_Instance()->Handle_Events(deltaTime);
}

void Engine::Update()
{
    float current_frame = (float)glfwGetTime();
    deltaTime = current_frame - last_frame;
    last_frame = current_frame;
    glfwPollEvents();

    Process_Input();

    // Update game state

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Render();

    glfwSwapBuffers(window_);
}

void Engine::Render()
{
    Texture tmp = Resource_Manager::Get_Texture("face");
    renderer->Draw_Sprite(tmp,
        glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
