/*************************************************************************************************************************************
* Title: graphics.cpp
* Author: Jennifer Assid
* Date of Creation: 23 March 2023
*
**************************************************************************************************************************************/
#include "graphics.h"
#include "engine.h"
#include "error_logging.h"

Graphics* Graphics::instance_ = nullptr;

void Graphics::Initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, true);

    engine_->Set_Window(glfwCreateWindow(engine_->Get_Width(),
        engine_->Get_Height(),
        engine_->Get_Window_Title().c_str(),
        nullptr,
        nullptr));
    glfwMakeContextCurrent(engine_->Get_Window());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Error_Logging::Get_Instance()->Record_Message("Failed to initialize GLAD",
            Error_Logging::Message_Level::ot_Error,
            "Engine",
            "Engine");
        abort();
    }

    glfwSetKeyCallback(engine_->Get_Window(), Keyboard_Callback);
    glfwSetFramebufferSizeCallback(engine_->Get_Window(), Frame_Buffer_Size_Callback);

    glViewport(0, 0, engine_->Get_Width(), engine_->Get_Height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Graphics::Shutdown()
{
    glfwTerminate();
}