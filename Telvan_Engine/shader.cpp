#include "shader.h"

#include <iostream>

void Shader::checked_compile_errors(unsigned int object, std::string type)
{
    int success;
    char info_buffer[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, info_buffer);
            std::cout << "| ERROR::Shader: Compile-time error: Type: " << type << "\n"
                << info_buffer << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, info_buffer);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << info_buffer << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}

Shader& Shader::Use()
{
    glUseProgram(ID);
    return *this;
}

void Shader::Compile(const char* vertex_source,
    const char* fragment_source,
    const char* geometry_source)
{
    unsigned int s_vertex, s_fragment, s_geometry;

    s_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s_vertex, 1, &vertex_source, NULL);
    glCompileShader(s_vertex);
    checked_compile_errors(s_vertex, "VERTEX");

    s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s_fragment, 1, &fragment_source, NULL);
    glCompileShader(s_fragment);
    checked_compile_errors(s_fragment, "FRAGMENT");

    if (geometry_source != nullptr)
    {
        s_geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(s_geometry, 1, &geometry_source, NULL);
        glCompileShader(s_geometry);
        checked_compile_errors(s_geometry, "GEOMETRY");
    }

    ID = glCreateProgram();
    glAttachShader(ID, s_vertex);
    glAttachShader(ID, s_fragment);
    if (geometry_source != nullptr) glAttachShader(ID, s_geometry);
    glLinkProgram(ID);
    checked_compile_errors(ID, "PROGRAM");

    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
    if (geometry_source != nullptr) glDeleteShader(s_geometry);
}

void Shader::Set_Float(const char* name, float value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::Set_Integer(const char* name, int value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::Set_Vector_2f(const char* name, float x, float y, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::Set_Vector_2f(const char* name, const glm::vec2& value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::Set_Vector_3f(const char* name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::Set_Vector_3f(const char* name, const glm::vec3& value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::Set_Vector_4f(const char* name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::Set_Vector_4f(const char* name, const glm::vec4& value, bool use_shader)
{
    if (use_shader)
        Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::Set_Matrix_4(const char* name, const glm::mat4& matrix, bool use_shader)
{
    if (use_shader)
        Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}