#include "texture_manager.h"
#include "error_logging.h"
#include "stb_image.h"

#include <filesystem>

Texture_Manager* Texture_Manager::instance_;

void Texture_Manager::Clear()
{
    for (auto iter : resources_)
        glDeleteTextures(1, &iter.second->ID);

    resources_.clear();
}
