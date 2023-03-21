#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <rapidjson/prettywriter.h>

#include "components.h"
#include "resource_manager.h"

class Sprite_Renderer : Component
{
private:
    Shader shader_;
    Texture texture_;
    unsigned int quad_VAO_;

private:
    void initialize_render_data();

public:
    Sprite_Renderer();
    ~Sprite_Renderer();

    void Render() override;

    void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
    void Read_From(rapidjson::Document& document) override;

    inline Shader& Get_Shader() { return shader_; }
    inline Texture& Get_Texture() { return texture_; }
    
    inline void Set_Shader(Shader& shader) { shader_ = shader; }
    inline void Set_Texture(Texture& texture) { texture_ = texture; }
};

#endif // !SPRITE_RENDERER_H
