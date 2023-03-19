#include "engine.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "error_logging.h"

#include "entity.h"
#include "transform.h"

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
    Resource_Manager::Clear();

    glfwTerminate();
}

Sprite_Renderer* renderer;
Entity* entity;
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
    entity = new Entity("Entity - Test");
    transform = entity->Get_Component<Transform>();
    transform->Set_Translation(glm::vec2(200.0f, 200.0f));
    transform->Set_Scale(glm::vec2(300.0f, 400.0f));
    transform->Set_Rotation(45.0f);

    input = Input::Get_Instance();
    input->Add_Binding(GLFW_KEY_W, Move_Up, Input::Callback_Type::cb_Press);
    input->Add_Binding(GLFW_KEY_S, Move_Down, Input::Callback_Type::cb_Press);
    input->Add_Binding(GLFW_KEY_A, Move_Left, Input::Callback_Type::cb_Press);
    input->Add_Binding(GLFW_KEY_D, Move_Right, Input::Callback_Type::cb_Press);

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
        transform->Get_Translation(), transform->Get_Scale(), transform->Get_Rotation(), glm::vec3(0.0f, 1.0f, 0.0f));
}
