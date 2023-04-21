/*************************************************************************************************************************************
* Title: collider.cpp
* Author: Jennifer Assid
* Date of Creation: 16 April 2023
*
**************************************************************************************************************************************/
#include "collider.h"

#include <glm/gtx/closest_point.hpp>

#include "shader_manager.h"

#include "error_logging.h"
#include "engine.h"
#include "transform.h"
#include "rigid_body.h"

bool Collider::circle_circle_check(Circle& a, Circle& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		return static_circle_circle_check(a, b);
	else if (a_rb == nullptr)
	{
		if (b_rb->Get_Current_Velocity() > 0.0f)
			return static_dynamic_circle_circle_check(b, a);
		else
			return static_circle_circle_check(a, b);
	}
	else
	{
		if (a_rb->Get_Current_Velocity() > 0.0f)
			return static_dynamic_circle_circle_check(a, b);
		else
			return static_circle_circle_check(a, b);
	}
}

bool Collider::static_circle_circle_check(Circle& static_a, Circle& static_b)
{
	Transform* a_trans = static_a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return false;

	Transform* b_trans = static_b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return false;

	glm::vec2 distance = b_trans->Get_Translation() - a_trans->Get_Translation();

	if (glm::length(distance) < static_a.Get_Radius() + static_b.Get_Radius())
		return true;

	return false;
}

bool Collider::static_dynamic_circle_circle_check(Circle& dynamic, Circle& static_circ)
{
	Rigid_Body* dynamic_rb = dynamic.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return false;

	Transform* static_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return false;

	Transform* dynamic_trans = dynamic.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return false;

	glm::vec2 direction = dynamic_rb->Get_Direction();

	glm::vec2 closest_point = glm::closestPointOnLine(static_trans->Get_Translation(),
		dynamic_trans->Get_Translation(),
		dynamic_trans->Get_Translation() + dynamic_rb->Get_Direction() * dynamic_rb->Get_Current_Velocity());

	glm::vec2 distance = static_trans->Get_Translation() - closest_point;

	if (glm::length(distance) < dynamic.Get_Radius() + static_circ.Get_Radius())
		return true;

	return false;
}

bool Collider::circle_AABB_check(Circle& a, AABB& b)
{
	Transform* a_trans = a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return false;

	Transform* b_trans = b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return false;

	glm::vec2 b_pos = b_trans->Get_Translation();
	glm::vec2 b_halfs = b.Get_Half_Length();

	glm::vec2 min = b_pos - b_halfs;
	glm::vec2 max = b_pos + b_halfs;

	glm::vec2 distance = a_trans->Get_Translation() - min;
	if (glm::length(distance) <= a.Get_Radius()) return true;

	distance = a_trans->Get_Translation() - glm::vec2(min.x, max.y);
	if (glm::length(distance) <= a.Get_Radius()) return true;

	distance = a_trans->Get_Translation() - glm::vec2(max.x, min.y);
	if (glm::length(distance) <= a.Get_Radius()) return true;

	distance = a_trans->Get_Translation() - max;
	if (glm::length(distance) <= a.Get_Radius()) return true;

	distance = a_trans->Get_Translation() - b_trans->Get_Translation();
	if (glm::length(distance) <= a.Get_Radius()) return true;

	return false;
}

bool Collider::AABB_AABB_check(AABB& a, AABB& b)
{
	Transform* a_trans = a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return false;

	Transform* b_trans = b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return false;

	glm::vec2 a_pos = a_trans->Get_Translation();
	glm::vec2 a_halfs = a.Get_Half_Length();

	glm::vec2 b_pos = b_trans->Get_Translation();
	glm::vec2 b_halfs = b.Get_Half_Length();

	// A above B
	if (a_pos.y + a_halfs.y > b_pos.y + b_halfs.y &&
		a_pos.y - a_halfs.y > b_pos.y + b_halfs.y)
		return false;

	// A below B
	if (a_pos.y + a_halfs.y < b_pos.y - b_halfs.y &&
		a_pos.y - a_halfs.y < b_pos.y - b_halfs.y)
		return false;

	// A right B
	if (a_pos.x + a_halfs.x > b_pos.x + b_halfs.x &&
		a_pos.x - a_halfs.x > b_pos.x + b_halfs.x)
		return false;

	// A left B
	if (a_pos.x + a_halfs.x < b_pos.x - b_halfs.x &&
		a_pos.x - a_halfs.x < b_pos.x - b_halfs.x)
		return false;

	return true;
}

void Collider::circle_circle_response(Circle& a, Circle& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		static_circle_circle_response(a, b);
	else if (a_rb == nullptr)
		dynamic_static_circle_circle_response(b, a);
	else if (b_rb == nullptr)
		dynamic_static_circle_circle_response(a, b);
	else
		dynamic_circle_circle_response(a, b);
}

void Collider::static_circle_circle_response(Circle& static_a, Circle& static_b)
{
	Transform* a_trans = static_a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return;

	Transform* b_trans = static_b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return;

	glm::vec2 midpoint = (b_trans->Get_Translation() + a_trans->Get_Translation()) / 2.0f;

	a_trans->Set_Translation(midpoint + static_a.Get_Radius() * glm::normalize(a_trans->Get_Translation() - b_trans->Get_Translation()));
	b_trans->Set_Translation(midpoint + static_b.Get_Radius() * glm::normalize(b_trans->Get_Translation() - a_trans->Get_Translation()));
}

void Collider::dynamic_static_circle_circle_response(Circle& dynamic, Circle& static_circ)
{
	Transform* dynamic_trans = dynamic.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return;

	Transform* static_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return;

	Rigid_Body* dynamic_rb = dynamic.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return;

	glm::vec2 n = dynamic_trans->Get_Translation() - static_trans->Get_Translation();
	n = glm::normalize(n);

	float p = (2.0f * glm::dot(dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction(), n)) 
		/ (dynamic_rb->Get_Mass() + dynamic_rb->Get_Mass());

	glm::vec2 w = dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction()
		- p * (dynamic_rb->Get_Mass() * n - dynamic_rb->Get_Mass() * n);

	glm::vec2 intersection = (static_trans->Get_Translation() - dynamic_trans->Get_Translation());
	float intersect_length = std::max((dynamic.Get_Radius() + static_circ.Get_Radius()) - glm::length(intersection), 
		0.0f);
	intersect_length += 5.0f;

	intersection = glm::normalize(intersection) * -intersect_length;

	dynamic_rb->Set_Current_Velocity(w + intersection);
}

void Collider::dynamic_circle_circle_response(Circle& dynamic_a, Circle& dynamic_b)
{

}

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
	model = glm::scale(model, glm::vec3(radius_ * 2.0f, radius_ * 2.0f, 1.0f));

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

bool Circle::Collision_Detection(Collider& other)
{
	switch (other.Get_Collider_Type())
	{
	case Collider_Type::col_Undefined:
		return false;
		break;
	case Collider_Type::col_Circle:
		return circle_circle_check(*this, (Circle&)other);
		break;
	case Collider_Type::col_AABB:
		return circle_AABB_check(*this, (AABB&)other);
		break;
	default:
		break;
	}

	return false;
}

void Circle::Collision_Response(Collider& other)
{
	switch (other.Get_Collider_Type())
	{
	case Collider_Type::col_Undefined:
		break;
	case Collider_Type::col_Circle:
		circle_circle_response(*this, (Circle&)other);
		break;
	case Collider_Type::col_AABB:
		break;
	default:
		break;
	}
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
	model = glm::scale(model, glm::vec3(half_length_.x * 2.0f, half_length_.y * 2.0f, 1.0f));

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

bool AABB::Collision_Detection(Collider& other)
{
	switch (other.Get_Collider_Type())
	{
	case Collider_Type::col_Undefined:
		return false;
		break;
	case Collider_Type::col_Circle:
		return circle_AABB_check((Circle&)other, *this);
		break;
	case Collider_Type::col_AABB:
		return AABB_AABB_check(*this, (AABB&)other);
		break;
	default:
		break;
	}

	return false;
}