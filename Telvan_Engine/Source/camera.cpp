#pragma once

#include "camera.h"
#include "transform.h"
#include "engine.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

glm::mat4 Camera::Get_Matrix()
{
	glm::mat4 result(1.0f);

	if (parent_ == nullptr) return result;
	
	Transform* transform = parent_->Get_Component<Transform>(Component_Type::ct_Transform);
	if (transform == nullptr) return result;
	
	glm::vec3 position = glm::vec3(transform->Get_Translation(),
		transform->Get_Z_Sorting_Value());
	position.y *= -1.0f;
	position.x -= Engine::Get_Instance()->Get_Width() * 0.5f;
	position.y -= Engine::Get_Instance()->Get_Height() * 0.5f;

	return glm::lookAt(position,
		position + glm::vec3(0,0, -zoom_),
		glm::vec3(0.0f, 1.0f, 0.0f));
}