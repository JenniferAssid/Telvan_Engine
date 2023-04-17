#include "collider.h"

#include "shader_manager.h"

#include "error_logging.h"
#include "engine.h"
#include "transform.h"

void Circle::initialize_circle_outline()
{
	unsigned int VBO;

	float angle = 360.0f / (float)segments_;

	float* vertices = new float[segments_ * 4];

	for (int i = 0; i < segments_; i++)
	{
		angle = 2.0f * 3.1415926f / float(segments_);
		vertices[i * 4] = 0.5f * cosf(i * angle);
		vertices[i * 4 + 1] = 0.5f * sinf(i * angle);

		vertices[i * 4 + 2] = 0.5f * cosf(i * angle + angle);
		vertices[i * 4 + 3] = 0.5f * sinf(i * angle + angle);
	}

	glGenVertexArrays(1, &this->line_VAO_);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * segments_ * 4, vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->line_VAO_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Circle::Start()
{
	shader_ = *Shader_Manager::Get_Instance()->Get_Resource("line");
	color_ = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
	initialize_circle_outline();
}

void Circle::Render()
{
	if (Engine::Get_Instance()->Get_Debug_Draw() == false) return;

	if (parent_ == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message("Parent not found",
			Error_Logging::Message_Level::ot_Information,
			"Collider-Circle",
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
			"Collider-Circle",
			"Render");
		return;
	}

	glm::vec2 position = transform->Get_Translation();
	glm::vec2 size = transform->Get_Scale();
	float rotate = transform->Get_Rotation();

	if (parent_->Get_Parent() != nullptr)
	{
		Transform* parent_transform = parent_->Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
		if (parent_transform != nullptr)
		{
			position += parent_transform->Get_Translation();
			size *= parent_transform->Get_Scale();
			rotate += parent_transform->Get_Rotation();
		}
	}

	position += offset_;
	position.y *= -1.0f;

	shader_.Use();
	shader_.Set_Matrix_4("projection",
		Engine::Get_Instance()->Projection);
	shader_.Set_Matrix_4("view", Engine::Get_Instance()->Get_Current_Camera()->Get_Matrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(radius_, radius_, 1.0f));

	position = Engine::Get_Instance()->Projection *
		Engine::Get_Instance()->Get_Current_Camera()->Get_Matrix() *
		model * glm::vec4(position, 0.0f, 1.0f);

	this->shader_.Set_Matrix_4("model", model);

	shader_.Set_Vector_4f("in_color", color_);
	
	glBindVertexArray(this->line_VAO_);
	glDrawArrays(GL_LINES, 0, segments_ * 4);
	glBindVertexArray(0);
	glUseProgram(0);
}

void AABB::initialize_square_outline()
{
	unsigned int VBO;
	float vertices[] = {
		// position     // tex coord
		-0.5f, 0.5f,     /*0.0f, 1.0f,*/     // top-left
		0.5f, 0.5f,     /*1.0f, 1.0f,*/     // top-right

		0.5f, 0.5f,     /*1.0f, 1.0f,*/     // top-right
		0.5f, -0.5f,     /*1.0f, 0.0f*/      // bottom-right

		0.5f, -0.5f,     /*1.0f, 0.0f*/      // bottom-right
		-0.5f, -0.5f,     /*0.0f, 0.0f,*/     // bottom-left

		-0.5f, -0.5f,     /*0.0f, 0.0f,*/     // bottom-left
		-0.5f, 0.5f     /*0.0f, 1.0f,*/     // top-left
	};

	glGenVertexArrays(1, &this->line_VAO_);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->line_VAO_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AABB::Start()
{
	shader_ = *Shader_Manager::Get_Instance()->Get_Resource("line");
	color_ = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
	initialize_square_outline();
}

void AABB::Render()
{
	if (Engine::Get_Instance()->Get_Debug_Draw() == false) return;

	if (parent_ == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message("Parent not found",
			Error_Logging::Message_Level::ot_Information,
			"Collider-Square",
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
			"Collider-Square",
			"Render");
		return;
	}

	glm::vec2 position = transform->Get_Translation();
	glm::vec2 size = transform->Get_Scale();
	float rotate = transform->Get_Rotation();

	if (parent_->Get_Parent() != nullptr)
	{
		Transform* parent_transform = parent_->Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
		if (parent_transform != nullptr)
		{
			position += parent_transform->Get_Translation();
			size *= parent_transform->Get_Scale();
			rotate += parent_transform->Get_Rotation();
		}
	}

	position += offset_;
	position.y *= -1.0f;

	shader_.Use();
	shader_.Set_Matrix_4("projection",
		Engine::Get_Instance()->Projection);
	shader_.Set_Matrix_4("view", Engine::Get_Instance()->Get_Current_Camera()->Get_Matrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(half_length_.x, half_length_.y, 1.0f));

	position = Engine::Get_Instance()->Projection *
		Engine::Get_Instance()->Get_Current_Camera()->Get_Matrix() *
		model * glm::vec4(position, 0.0f, 1.0f);

	this->shader_.Set_Matrix_4("model", model);

	shader_.Set_Vector_4f("in_color", color_);

	glBindVertexArray(this->line_VAO_);
	glDrawArrays(GL_LINES, 0, 4 * 4);
	glBindVertexArray(0);
	glUseProgram(0);
}