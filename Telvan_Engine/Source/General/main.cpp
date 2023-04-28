/*************************************************************************************************************************************
* Title: main.cpp
* Author: Jennifer Assid
* Date of Creation: 13 March 2023
*
**************************************************************************************************************************************/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine.h"
#include "error_logging.h"
#include "serialize.h"
#include <iostream>


int main(int argc, char* argv[])
{
    Engine* engine = Engine::Get_Instance();

    if (engine == nullptr) abort();

    engine->Initialize();

    while (engine->Should_Engine_Shutdown() == false)
    {
        engine->Update();
    }

    engine->Active = false;

    engine->Shutdown();
}
