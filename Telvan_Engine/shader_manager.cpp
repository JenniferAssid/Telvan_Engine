#include "shader_manager.h"
#include "error_logging.h"

#include <sstream>

Shader_Manager* Shader_Manager::instance_;

Shader* Shader_Manager::load_shader_from_file(const char* s_vertex,
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

    Shader* shader = new Shader();
    shader->Compile(code_vertex, code_fragment, (s_geometry != nullptr) ? code_geometry : nullptr);
    return shader;
}

void Shader_Manager::open_files(std::string path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory() == true) open_files(entry.path().string() + "/");
        else
        {
            if (entry.path().extension() == ".vert") vertexs_.push_back(entry.path());
            else if (entry.path().extension() == ".frag") fragments_.push_back(entry.path());
        }
    }
}

void Shader_Manager::Initialize()
{
    open_files(master_path_);

    for (int i = 0; i < vertexs_.size(); i++)
    {
        Shader* tmp = load_shader_from_file(vertexs_[i].string().c_str(),
            fragments_[i].string().c_str(),
            nullptr);
        tmp->Name = vertexs_[i].stem().string();
        resources_[tmp->Name] = tmp;
    }

    vertexs_.clear();
    fragments_.clear();
}

void Shader_Manager::Clear()
{
    for (auto iter : resources_)
        glDeleteTextures(1, &iter.second->ID);

    resources_.clear();
}