/*************************************************************************************************************************************
* Title: spriterenderer.h
* Author: Jennifer Assid
* Date of Creation: 16 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <rapidjson/prettywriter.h>

#include "components.h"

#include "texture.h"
#include "shader.h"

class Sprite_Renderer : Component
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
    Shader shader_;
    Texture texture_;
    unsigned int quad_VAO_;

protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
    void initialize_render_data();

protected:
public:
    Sprite_Renderer();
    ~Sprite_Renderer();

    Component* Clone() override;

    void Render() override;

    void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values) override;
    void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer) override;

    inline Shader& Get_Shader() { return shader_; }
    inline Texture& Get_Texture() { return texture_; }
    
    inline void Set_Shader(Shader& shader) { shader_ = shader; }
    inline void Set_Texture(Texture& texture) { texture_ = texture; }
};

#endif // !SPRITE_RENDERER_H
