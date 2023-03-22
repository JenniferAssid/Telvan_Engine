#include "engine.h"
#include "error_logging.h"

#include "shader_manager.h"
#include "texture_manager.h"
#include "scene_manager.h"
#include "prefab_manager.h"
#include "entity_manager.h"



#include "entity.h"
#include "transform.h"
#include "input_controller.h"
#include "sprite_renderer.h"

#include <iostream>

Engine* Engine::instance_ = nullptr;

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
        Error_Logging::Get_Instance()->Record_Message("Failed to initialize GLAD",
            Error_Logging::Message_Level::ot_Error,
            "Engine",
            "Engine");
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
    Texture_Manager::Get_Instance()->Clear();
    Shader_Manager::Get_Instance()->Clear();

    glfwTerminate();
}

Input* input;
Entity_Manager* entity_manager;
Shader_Manager* shader_manager;
Texture_Manager* texture_manager;
Prefab_Manager* prefab_manager;
Scene_Manager* scene_manager;

Entity* entity;
Entity* test;


void Engine::Initialize()
{
    deltaTime = last_frame = 0.0f;
    
    input = Input::Get_Instance();
    shader_manager = Shader_Manager::Get_Instance();
    texture_manager = Texture_Manager::Get_Instance();
    prefab_manager = Prefab_Manager::Get_Instance();
    scene_manager = Scene_Manager::Get_Instance();

    shader_manager->Initialize("./Assets/Shaders/");
    texture_manager->Initialize("./Assets/Textures/");
    scene_manager->Initialize("./Data/Scenes/");
    prefab_manager->Initialize("./Data/Prefabs/");

    Scene* scene = scene_manager->Get_Resource("Test_Scene");
    scene->Load();

    Shader shader = *shader_manager->Get_Resource("default");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width_),
        static_cast<float>(this->height_), 0.0f, -1.0f, 1.0f);
    shader.Use().Set_Integer("tex", 0);
    shader.Set_Matrix_4("projection", projection);

    Entity_Manager::Get_Instance()->Start();
}

void Engine::Update()
{
    float current_frame = (float)glfwGetTime();
    deltaTime = current_frame - last_frame;
    last_frame = current_frame;
    glfwPollEvents();


    // Update game state
    Entity_Manager::Get_Instance()->Update(deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Render();

    glfwSwapBuffers(window_);
}

void Engine::Render()
{
    Entity_Manager::Get_Instance()->Render();
}
