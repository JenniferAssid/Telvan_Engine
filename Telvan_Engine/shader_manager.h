#pragma once

#ifndef SHADER_MANAGER_CLASS_H
#define SHADER_MANAGER_CLASS_H

#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include "shader.h"
#include "resource_manager.h"

class Shader_Manager : public Resource_Manager<Shader>
{
protected:
    Shader_Manager() : Resource_Manager<Shader>("Shader_Manager", "./Assets/Shaders/") {}
    static Shader_Manager* instance_;

    Shader* load_shader_from_file(const char* s_vertex,
        const char* s_fragment,
        const char* s_geometry = nullptr);

    void open_files(std::string path) override;
    
    std::vector<std::filesystem::path> vertexs_;
    std::vector<std::filesystem::path> fragments_;

public:
    static inline Shader_Manager* Get_Instance()
    {
        if (instance_ == nullptr)
            instance_ = new Shader_Manager();

        return instance_;
    }

    void Initialize() override;

    void Clear() override;
};

#endif // !SHADER_MANAGER_CLASS_H

