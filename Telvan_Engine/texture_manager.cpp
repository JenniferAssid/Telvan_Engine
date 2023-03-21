#include "texture_manager.h"
#include "error_logging.h"
#include "stb_image.h"

#include <filesystem>

std::map<std::string, Texture> Texture_Manager::textures_;
Texture_Manager* Texture_Manager::instance_;

Texture Texture_Manager::load_texture_from_file(const char* file,
    bool alpha)
{
    Texture texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    int width, height, nr_channels;
    unsigned char* data = stbi_load(file, &width, &height, &nr_channels, 0);

    texture.Generate(width, height, data);

    stbi_image_free(data);

    return texture;
}

void Texture_Manager::open_files(std::string path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory() == true) open_files(entry.path().string() + "/");
        else
        {
            Load_Texture(entry.path().string().c_str(),
                true,
                entry.path().stem().string());
        }
    }
}

Texture_Manager* Texture_Manager::Get_Instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Texture_Manager();
    }
    return instance_;
}

void Texture_Manager::Initialize()
{
    std::string path = "Assets/Textures/";
    open_files(path);
}

Texture Texture_Manager::Load_Texture(const char* file,
    bool alpha,
    std::string name)
{
    Texture tmp = load_texture_from_file(file, alpha);
    tmp.Name = name;
    textures_[name] = tmp;
    return textures_[name];
}

Texture Texture_Manager::Get_Texture(std::string name)
{
    return textures_[name];
}

void Texture_Manager::Clear()
{
    for (auto iter : textures_)
        glDeleteTextures(1, &iter.second.ID);
}
