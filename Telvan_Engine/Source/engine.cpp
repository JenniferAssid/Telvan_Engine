#include "engine.h"

#include "error_logging.h"
#include "graphics.h"
#include "shader_manager.h"
#include "texture_manager.h"
#include "scene_manager.h"
#include "prefab_manager.h"
#include "entity_manager.h"
#include "collider_manager.h"

#include "entity.h"
#include "transform.h"
#include "collider.h"
#include "input_controller.h"
#include "sprite_renderer.h"

#include <iostream>

Engine* Engine::instance_ = nullptr;

Engine::Engine(unsigned int width,
    unsigned height)
{
    // Initialize the class variables
    width_ = width;
    height_ = height;
    deltaTime = last_frame = 0.0f;
    debug_draw_ = true;

    // Register the system in the order the need to be INITIALIZED
    // in top-to-bottom order
    Register_System((System*)Error_Logging::Get_Instance());
    Register_System((System*)Graphics::Get_Instance());
    Register_System((System*)Shader_Manager::Get_Instance());
    Register_System((System*)Texture_Manager::Get_Instance());
    Register_System((System*)Prefab_Manager::Get_Instance());
    Register_System((System*)Scene_Manager::Get_Instance());
}

void Engine::calculate_delta_time()
{
    float current_frame = (float)glfwGetTime();
    deltaTime = current_frame - last_frame;
    last_frame = current_frame;
}

Engine* Engine::Get_Instance()
{
    if (instance_ == nullptr)
        instance_ = new Engine(800, 600);

    return instance_;
}

Engine::~Engine()
{
    glfwTerminate();
}

Entity_Manager* entity_manager;
Shader_Manager* shader_manager;
Texture_Manager* texture_manager;
Prefab_Manager* prefab_manager;
Scene_Manager* scene_manager;

Entity* entity;
Entity* test;

bool on_enter_test(Collider& a, Collider& b)
{
    return true;
}

bool on_exit_test(Collider& a, Collider& b)
{
    return false;
}

bool while_triggered_test(Collider& a, Collider& b)
{
    return true;
}

void Engine::Initialize()
{
    // Initialize the class members
    deltaTime = last_frame = 0.0f;
    window_title_ = "Telvan Engine";
    
    // Manager pointers (for testing purposes)
    shader_manager = (Shader_Manager*)Find_System("Shader_Manager");
    texture_manager = (Texture_Manager*)Find_System("Texture_Manager");
    prefab_manager = (Prefab_Manager*)Find_System("Prefab_Manager");
    scene_manager = (Scene_Manager*)Find_System("Scene_Manager");
    entity_manager = Entity_Manager::Get_Instance();

    // Initialize the systems in the order they were registered
    for (System* system : systems_)
    {
        Error_Logging::Get_Instance()->Record_Message(
            "Initializing System",
            Error_Logging::Message_Level::ot_Information,
            system->Get_Name(),
            "Initialize"
        );
        system->Initialize();
    }

    /************************ TESTING CODE BELOW ********************************/
    
    // Faux tileset placement
    float margin = 25.0f;
    glm::vec2 tmp_position(0.0f);
    entity = new Entity(*prefab_manager->Get_Resource("Floor"));
    entity->Get_Component<Transform>(Component_Type::ct_Transform)->Set_Translation(glm::vec2(tmp_position.x + margin,
        tmp_position.y + margin));
    Circle* circle = entity->Add_Component<Circle>(Component_Type::ct_Collider);
    circle->Set_On_Enter(on_enter_test);
    circle->Set_On_Exit(on_exit_test);
    circle->Set_Is_Trigger(true);
    entity_manager->Add_Entity(entity);

    entity = new Entity(*prefab_manager->Get_Resource("Player_Controller"));
    //entity->Add_Component<Circle>(Component_Type::ct_Collider);
    entity->Add_Component<Circle>(Component_Type::ct_Collider);
    current_camera_ = entity->Add_Component<Camera>(Component_Type::ct_Camera);
    entity_manager->Add_Entity(entity);

    Entity_Manager::Get_Instance()->Start();
    Collider_Manager::Get_Instance()->Start();
}

void Engine::Update()
{
    Projection = glm::ortho(0.0f,
        (float)width_,
        (float)height_,
        0.0f,
        -1.0f,
        1.0f);

    calculate_delta_time();
    glfwPollEvents();

    Entity_Manager::Get_Instance()->Update(deltaTime);

    Render();
}

void Engine::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwGetWindowSize(window_, &width_, &height_);

    Entity_Manager::Get_Instance()->Render();
    
    if (debug_draw_ == true)
        Collider_Manager::Get_Instance()->Render();

    glfwSwapBuffers(window_);
}

void Engine::Shutdown()
{
    // Shutdown registered systems in REVERSE order
    for (int i = systems_.size() - 1;
        i >= 0;
        i--)
    {
        Error_Logging::Get_Instance()->Record_Message(
            "Shutting system down",
            Error_Logging::Message_Level::ot_Information,
            systems_[i]->Get_Name(),
            "Shutdown"
        );
        systems_[i]->Shutdown();
    }
}
