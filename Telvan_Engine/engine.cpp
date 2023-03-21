#include "engine.h"
#include "shader_manager.h"
#include "sprite_renderer.h"
#include "prefab_manager.h"
#include "entity_manager.h"
#include "error_logging.h"
#include "scene.h"
#include "entity.h"
#include "transform.h"
#include "input_controller.h"

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

Sprite_Renderer* renderer;
Entity* entity;
Entity* test;
Transform* transform;
Input* input;

void Move_Up(float dT)
{
    glm::vec2 pos = transform->Get_Translation();
    pos.y -= 5.0f;
    transform->Set_Translation(pos);
}
void Move_Down(float dT)
{
    glm::vec2 pos = transform->Get_Translation();
    pos.y += 5.0f;
    transform->Set_Translation(pos);
}
void Move_Left(float dT)
{
    glm::vec2 pos = transform->Get_Translation();
    pos.x -= 5.0f;
    transform->Set_Translation(pos);
}
void Move_Right(float dT)
{
    glm::vec2 pos = transform->Get_Translation();
    pos.x += 5.0f;
    transform->Set_Translation(pos);
}

void Engine::Initialize()
{
    deltaTime = last_frame = 0.0f;
    /*entity = new Entity("Entity - Test");*/
    
    input = Input::Get_Instance();
    /*input->Add_Binding(GLFW_KEY_W, Move_Up, Input::Callback_Type::cb_Down);
    input->Add_Binding(GLFW_KEY_S, Move_Down, Input::Callback_Type::cb_Down);
    input->Add_Binding(GLFW_KEY_A, Move_Left, Input::Callback_Type::cb_Down);
    input->Add_Binding(GLFW_KEY_D, Move_Right, Input::Callback_Type::cb_Down);*/

    Shader_Manager* shader_manager = Shader_Manager::Get_Instance();

    Shader shader = shader_manager->Load_Shader("Assets/Shaders/default.vert", "Assets/Shaders/default.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width_),
        static_cast<float>(this->height_), 0.0f, -1.0f, 1.0f);
    shader.Use().Set_Integer("tex", 0);
    shader.Set_Matrix_4("projection", projection);
    /*Texture texture = Texture_Manager::Get_Instance()->Load_Texture("Assets/Textures/awesomeface.png", true, "face");*/
    Texture_Manager::Get_Instance()->Initialize();

    Prefab_Manager::Get_Instance()->Initialize();

    /*Scene* scene = new Scene("Test_Scene");
    scene->Load();*/

    entity = Prefab_Manager::Get_Instance()->Get_Prefab("Entity - Test");

    test = new Entity(*entity);
    test->Set_Name("Clone");
    test->Remove_Component(Component_Type::ct_Input_Controller);

    Entity_Manager::Get_Instance()->Add_Entity(entity);
    Entity_Manager::Get_Instance()->Add_Entity(test);

    /*transform = entity->Get_Component<Transform>(Component_Type::ct_Transform);
    renderer = entity->Get_Component<Sprite_Renderer>(Component_Type::ct_Sprite_Renderer);

    std::ofstream ofs("./Data/Scenes/Test_Scene.json");

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();
    writer.Key("Test_Scene");
    writer.StartArray();

    entity->Write_To(false, &writer);
    test->Write_To(false, &writer);

    writer.EndArray();
    writer.EndObject();

    ofs.clear();
    ofs << sb.GetString();
    ofs.close();*/

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
    /*renderer->Draw_Sprite(tmp,
        transform->Get_Translation(), transform->Get_Scale(), transform->Get_Rotation(), glm::vec3(0.0f, 1.0f, 0.0f));*/
    Entity_Manager::Get_Instance()->Render();
}
