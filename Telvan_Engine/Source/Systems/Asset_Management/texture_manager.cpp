/*************************************************************************************************************************************
* Title: texture_manager.cpp
* Author: Jennifer Assid
* Date of Creation: 20 March 2023
*
**************************************************************************************************************************************/
#include "texture_manager.h"
#include "error_logging.h"
#include "stb_image.h"

#include "texture.h"

#include <filesystem>

Texture_Manager* Texture_Manager::instance_;

void Texture_Manager::Clear()
{
    for (auto iter : resources_)
        glDeleteTextures(1, &iter.second->ID);

    resources_.clear();
}
