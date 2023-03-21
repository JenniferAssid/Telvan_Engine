#pragma once

#ifndef SHADER_MANAGER_CLASS_H
#define SHADER_MANAGER_CLASS_H

#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include "shader.h"

class Shader_Manager
{
private:
    Shader_Manager() {}
    static Shader_Manager* instance_;

    static std::map<std::string, Shader> shaders_;

    static Shader load_shader_from_file(const char* s_vertex,
        const char* s_fragment,
        const char* s_geometry = nullptr);

    void open_files(std::string path,
        std::vector<std::filesystem::path>& vertex,
        std::vector<std::filesystem::path>& fragment);

public:
    static Shader_Manager* Get_Instance();

    void Initialize();

    static Shader Load_Shader(const char* s_vertex,
        const char* s_fragment,
        const char* s_geometry,
        std::string name);

    static Shader Get_Shader(std::string name);

    static void Clear();
};

#endif // !SHADER_MANAGER_CLASS_H

