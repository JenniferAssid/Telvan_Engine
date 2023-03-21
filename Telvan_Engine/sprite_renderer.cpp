#include "sprite_renderer.h"
#include "shader_manager.h"
#include "entity.h"
#include "transform.h"
#include "error_logging.h"

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

Sprite_Renderer::Sprite_Renderer() : Component(Component_Type::ct_Sprite_Renderer)
{
    this->initialize_render_data();
}

Sprite_Renderer::~Sprite_Renderer()
{
    glDeleteVertexArrays(1, &this->quad_VAO_);
}

Component* Sprite_Renderer::Clone()
{
    Sprite_Renderer* sprite_renderer = new Sprite_Renderer();

    Shader shader = Shader_Manager::Get_Instance()->Get_Shader(shader_.Name);
    sprite_renderer->Set_Shader(shader);
    Texture texture = Texture_Manager::Get_Instance()->Get_Texture(texture_.Name);
    sprite_renderer->Set_Texture(texture);
    
    return (Component*)sprite_renderer;
}

void Sprite_Renderer::Render()
{
    if (parent_ == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message("Parent not found",
            Error_Logging::Message_Level::ot_Information,
            "Sprite_Renderer",
            "Render");
        return;
    }

    Transform* transform = parent_->Get_Component<Transform>(Component_Type::ct_Transform);

    if (transform == nullptr)
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Transform not found for entity \"%s\"", 
                                                            parent_->Get_Name().c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Sprite_Renderer",
            "Render");
        return;
    }

    glm::vec2 position = transform->Get_Translation();
    glm::vec2 size = transform->Get_Scale();
    float rotate = transform->Get_Rotation();

    // prepare transformations
    shader_.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader_.Set_Matrix_4("model", model);
    this->shader_.Set_Vector_3f("tex_color", glm::vec3(1.0f));

    glActiveTexture(GL_TEXTURE0);
    texture_.Bind();

    glBindVertexArray(this->quad_VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite_Renderer::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
    writer.Key("sprite_renderer");
    writer.StartObject();

    writer.Key("shader");
    writer.String(shader_.Name.c_str());

    writer.Key("texture");
    writer.String(texture_.Name.c_str());

    writer.EndObject();
}

void Sprite_Renderer::Read_From(rapidjson::Document& document)
{
    if (document.HasMember("sprite_renderer") == false) return;

    if (document["sprite_renderer"].GetObject().HasMember("shader") &&
        document["sprite_renderer"]["shader"].IsString())
    {
        const rapidjson::Value& shader = document["sprite_renderer"]["shader"];

        shader_ = Shader_Manager::Get_Instance()->Get_Shader(shader.GetString());
    }

    if (document["sprite_renderer"].GetObject().HasMember("texture") &&
        document["sprite_renderer"]["texture"].IsString())
    {
        const rapidjson::Value& texture = document["sprite_renderer"]["texture"];

        texture_ = Texture_Manager::Get_Instance()->Get_Texture(texture.GetString());
    }
}
