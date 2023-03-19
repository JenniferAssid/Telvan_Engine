#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine.h"
#include "error_logging.h"
#include "serialize.h"
#include <iostream>


int main(int argc, char* argv[])
{
    Engine* engine = Engine::Get_Instance();
    Error_Logging* error_logging = Error_Logging::Get_Instance();

    Serialize* test = new Serialize("Data/Test.json");

    error_logging->Initialize();
    engine->Initialize();

    while (engine->Should_Engine_Shutdown() == false)
    {
        engine->Update();
    }

    error_logging->Shutdown();
}
