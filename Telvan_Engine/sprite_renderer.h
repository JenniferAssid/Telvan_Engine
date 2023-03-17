#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

class Sprite_Renderer
{
private:
    Shader shader_;
    unsigned int quad_VAO_;

private:
    void initialize_render_data();

public:
    Sprite_Renderer(Shader& shader);
    ~Sprite_Renderer();

    void Draw_Sprite(Texture& texture,
        glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f),
        float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));
};

#endif // !SPRITE_RENDERER_H
