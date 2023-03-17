#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine.h"
#include <iostream>

Engine* Engine::instance_ = nullptr;
Input* Input::instance_ = nullptr;

int main(int argc, char* argv[])
{
    Engine* engine = Engine::Get_Instance();

    engine->Initialize();    

    while (engine->Should_Engine_Shutdown() == false)
    {
        engine->Update();
    }
}
