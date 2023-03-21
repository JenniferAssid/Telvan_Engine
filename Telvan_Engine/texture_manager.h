#pragma once

#ifndef TEXTURE_MANAGER_CLASS_H
#define TEXTURE_MANAGER_CLASS_H

#include <map>
#include <string>

#include "texture.h"

class Texture_Manager
{
private:
    Texture_Manager() {}
    static Texture_Manager* instance_;

    static std::map<std::string, Texture> textures_;

    static Texture load_texture_from_file(const char* file,
        bool alpha);

    void open_files(std::string path);

public:
    static Texture_Manager* Get_Instance();

    void Initialize();

    static Texture Load_Texture(const char* file,
        bool alpha,
        std::string name);

    static Texture Get_Texture(std::string name);

    static void Clear();
};

#endif // !SHADER_MANAGER_CLASS_H

