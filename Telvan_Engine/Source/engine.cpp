#include "engine.h"

#include "error_logging.h"
#include "graphics.h"
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

    Register_System((System*)Error_Logging::Get_Instance());
    Register_System((System*)Graphics::Get_Instance());
    Register_System((System*)Shader_Manager::Get_Instance());
    Register_System((System*)Texture_Manager::Get_Instance());
    Register_System((System*)Prefab_Manager::Get_Instance());
    Register_System((System*)Scene_Manager::Get_Instance());
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
    window_title_ = "Telvan Engine";
    
    input = Input::Get_Instance();
    shader_manager = (Shader_Manager*)Find_System("Shader_Manager");
    texture_manager = (Texture_Manager*)Find_System("Texture_Manager");
    prefab_manager = (Prefab_Manager*)Find_System("Prefab_Manager");
    scene_manager = (Scene_Manager*)Find_System("Scene_Manager");
    entity_manager = Entity_Manager::Get_Instance();

    for (System* system : systems_)
        system->Initialize();

    /*Scene* scene = scene_manager->Get_Resource("Test_Scene");
    scene->Load();*/

    entity = new Entity(*prefab_manager->Get_Resource("Entity_With_Instance_Children"));
    /*entity = new Entity(*prefab_manager->Get_Resource("Player_Controller"));
    entity->Set_Name("Entity_With_Instance_Children");
    entity->Add_Child(new Entity("Child_Instance_1", ""));
    entity->Get_Children()[0]->Add_Component<Transform>(Component_Type::ct_Transform)->Set_Scale(glm::vec2(0.5f, 0.5f));
    entity->Get_Children()[0]->Get_Component<Transform>(Component_Type::ct_Transform)->Set_Rotation(-20.0f);
    entity->Get_Children()[0]->Add_Component<Sprite_Renderer>(Component_Type::ct_Sprite_Renderer)->Set_Shader(
        *shader_manager->Get_Resource("default")
    );
    entity->Get_Children()[0]->Add_Component<Sprite_Renderer>(Component_Type::ct_Sprite_Renderer)->Set_Texture(
        *texture_manager->Get_Resource("awesomeface")
    );*/
    //test = new Entity(*prefab_manager->Get_Resource("Write_Out_Test"));

    //Input_Controller* input_controller = entity->Add_Component<Input_Controller>(Component_Type::ct_Input_Controller);

    //entity->Write_To(false, false);

    entity_manager->Add_Entity(entity);
    //entity_manager->Add_Entity(test);

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

void Engine::Shutdown()
{
    for (System* system : systems_)
        system->Shutdown();
}
