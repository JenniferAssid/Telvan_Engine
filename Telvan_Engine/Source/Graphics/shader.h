/*************************************************************************************************************************************
* Title: shader.h
* Author: Jennifer Assid
* Date of Creation: 13 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
protected:
public:
    unsigned int ID;
    std::string Name;

/************************************************************ FUNCTIONS **************************************************************/
private:
    void checked_compile_errors(unsigned int object, std::string type, std::string function_name);

protected:
public:
    Shader() : ID(-1), Name("Shader_NoName") {}
    Shader(std::filesystem::path path) : ID(-1), Name(path.stem().string()) {}
    
    Shader& Use();
    void Compile(const char* vertex_source, 
        const char* fragment_source, 
        const char* geometry_source = nullptr);

    // Utility Function
    void Set_Float(const char* name, float value, bool use_shader = false);
    void Set_Integer(const char* name, int value, bool use_shader = false);
    void Set_Vector_2f(const char* name, float x, float y, bool use_shader = false);
    void Set_Vector_2f(const char* name, const glm::vec2& value, bool use_shader = false);
    void Set_Vector_3f(const char* name, float x, float y, float z, bool use_shader = false);
    void Set_Vector_3f(const char* name, const glm::vec3& value, bool use_shader = false);
    void Set_Vector_4f(const char* name, float x, float y, float z, float w, bool use_shader = false);
    void Set_Vector_4f(const char* name, const glm::vec4& value, bool use_shader = false);
    void Set_Matrix_4(const char* name, const glm::mat4& matrix, bool use_shader = false);
};

#endif