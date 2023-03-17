#include "sprite_renderer.h"

#include <glad/glad.h>

void Sprite_Renderer::initialize_render_data()
{
    unsigned int VBO;
    float vertices[] = {
        // position     // tex coord
        0.0f, 1.0f,     0.0f, 1.0f,     // top-left
        1.0f, 0.0f,     1.0f, 0.0f,     // bottom-right
        0.0f, 0.0f,     0.0f, 0.0f,     // bottom-left

        0.0f, 1.0f,     0.0f, 1.0f,     // top-left
        1.0f, 1.0f,     1.0f, 1.0f,     // top-right
        1.0f, 0.0f,     1.0f, 0.0f      // bottom-left
    };

    glGenVertexArrays(1, &this->quad_VAO_);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quad_VAO_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Sprite_Renderer::Sprite_Renderer(Shader& shader)
{
    this->shader_ = shader;
    this->initialize_render_data();
}

Sprite_Renderer::~Sprite_Renderer()
{
    glDeleteVertexArrays(1, &this->quad_VAO_);
}

void Sprite_Renderer::Draw_Sprite(Texture& texture, 
    glm::vec2 position,
    glm::vec2 size, 
    float rotate, 
    glm::vec3 color)
{
    // prepare transformations
    this->shader_.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader_.Set_Matrix_4("model", model);
    this->shader_.Set_Vector_3f("tex_color", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quad_VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
