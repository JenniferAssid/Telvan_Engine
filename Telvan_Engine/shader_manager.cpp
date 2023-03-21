#include "shader_manager.h"
#include "error_logging.h"

#include <sstream>



std::map<std::string, Shader> Shader_Manager::shaders_;
Shader_Manager* Shader_Manager::instance_;

Shader Shader_Manager::load_shader_from_file(const char* s_vertex,
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

void Shader_Manager::open_files(std::string path, 
    std::vector<std::filesystem::path>& vertex,
    std::vector<std::filesystem::path>& fragment)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory() == true) open_files(entry.path().string() + "/", vertex, fragment);
        else
        {
            if (entry.path().extension() == ".vert") vertex.push_back(entry.path());
            else if (entry.path().extension() == ".frag") fragment.push_back(entry.path());
        }
    }
}

Shader_Manager* Shader_Manager::Get_Instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Shader_Manager();
    }
    return instance_;
}

void Shader_Manager::Initialize()
{
    std::vector<std::filesystem::path> vertex_shaders;
    std::vector<std::filesystem::path> fragment_shaders;

    std::string path = "Assets/Shaders/";
    open_files(path, vertex_shaders, fragment_shaders);

    unsigned int vertex_step = 0;
    unsigned int fragment_step = 0;

    for (unsigned int i = 0; i < vertex_shaders.size(); i++)
    {
        Load_Shader(vertex_shaders[i].string().c_str(),
            fragment_shaders[i].string().c_str(),
            nullptr,
            vertex_shaders[i].stem().string().c_str());
    }
}

Shader Shader_Manager::Load_Shader(const char* s_vertex,
    const char* s_fragment,
    const char* s_geometry,
    std::string name)
{
    Shader tmp = load_shader_from_file(s_vertex, s_fragment, s_geometry);
    tmp.Name = name;
    shaders_[name] = tmp;
    return shaders_[name];
}

Shader Shader_Manager::Get_Shader(std::string name)
{
    return shaders_[name];
}

void Shader_Manager::Clear()
{
    for (auto iter : shaders_)
        glDeleteTextures(1, &iter.second.ID);
}