#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <map>
#include <string>

//TODO: Look into abstracting this into a dynamic resource manager

#include "texture.h"
#include "shader.h"

class Resource_Manager
{
private:
    Resource_Manager() {}

    static Shader load_shader_from_file(const char* s_vertex, 
        const char* s_fragment, 
        const char* s_geometry = nullptr);

    static Texture load_texture_from_file(const char* file,
        bool alpha);

public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture> Textures;

    static Shader Load_Shader(const char* s_vertex,
        const char* s_fragment,
        const char* s_geometry,
        std::string name);

    static Shader Get_Shader(std::string name);
    
    static Texture Load_Texture(const char* file,
        bool alpha,
        std::string name);

    static Texture Get_Texture(std::string name);

    static void Clear();
};

#endif // !RESOURCE_MANAGER_CLASS_H

