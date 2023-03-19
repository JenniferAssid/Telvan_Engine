#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"
#include "error_logging.h"

std::map<std::string, Texture> Resource_Manager::Textures;
std::map<std::string, Shader> Resource_Manager::Shaders;

Shader Resource_Manager::load_shader_from_file(const char* s_vertex,
    const char* s_fragment,
    const char* s_geometry)
{
    std::string vertex;
    std::string fragment;
    std::string geometry;

    try
    {
        std::ifstream vertex_shader(s_vertex);
        std::ifstream fragment_shader(s_fragment);
        std::stringstream ss_vertex, ss_fragement;

        ss_vertex << vertex_shader.rdbuf();
        ss_fragement << fragment_shader.rdbuf();

        vertex_shader.close();
        fragment_shader.close();

        vertex = ss_vertex.str();
        fragment = ss_fragement.str();
        
        if (s_geometry != nullptr)
        {
            std::ifstream geometry_shader(s_vertex);
            std::stringstream ss_geometry;

            ss_geometry << geometry_shader.rdbuf();

            geometry_shader.close();

            geometry = ss_geometry.str();
        }
    }
    catch (std::exception e)
    {
        Error_Logging::Get_Instance()->Record_Message("Failed to read shader files",
            Error_Logging::Message_Level::ot_Warning,
            "Resource_Manager",
            "load_shader_from_file");
        /*std::cout << "| ERROR::Shader: Failed to read shader files" << std::endl;*/
    }

    const char* code_vertex = vertex.c_str();
    const char* code_fragment = fragment.c_str();
    const char* code_geometry = geometry.c_str();

    Shader shader;
    shader.Compile(code_vertex, code_fragment, (s_geometry != nullptr) ? code_geometry : nullptr);
    return shader;
}

Texture Resource_Manager::load_texture_from_file(const char* file,
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

//TODO: Add error checking for the key accessing on the maps
Shader Resource_Manager::Load_Shader(const char* s_vertex,
    const char* s_fragment,
    const char* s_geometry,
    std::string name)
{
    Shaders[name] = load_shader_from_file(s_vertex, s_fragment, s_geometry);
    return Shaders[name];
}

Shader Resource_Manager::Get_Shader(std::string name)
{
    return Shaders[name];
}

Texture Resource_Manager::Load_Texture(const char* file,
    bool alpha,
    std::string name)
{
    Textures[name] = load_texture_from_file(file, alpha);
    return Textures[name];
}

Texture Resource_Manager::Get_Texture(std::string name)
{
    return Textures[name];
}

void Resource_Manager::Clear()
{
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}
