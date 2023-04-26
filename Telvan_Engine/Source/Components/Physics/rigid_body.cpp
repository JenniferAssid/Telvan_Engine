/*************************************************************************************************************************************
* Title: rigid_body.cpp
* Author: Jennifer Assid
* Date of Creation: 17 April 2023
*
**************************************************************************************************************************************/

#include "rigid_body.h"
#include "transform.h"

void Rigid_Body::Pre_Update(float dT)
{
	Transform* transform = parent_->Get_Component<Transform>(Component_Type::ct_Transform);

	if (transform == nullptr) return;

	direction_ = (total_force_ / mass_);

	current_velocity_ = std::min(glm::length(direction_), max_velocity_) * dT;

	total_force_ *= 0.8f;

	// Apply the velocity to the position
	glm::vec2 position = transform->Get_Translation();

	position += direction_ * current_velocity_;

	transform->Set_Translation(position);
}

void Rigid_Body::Add_Force(glm::vec2 force)
{
	total_force_ += force;
}

void Rigid_Body::Set_Current_Velocity(glm::vec2 velocity)
{
	total_force_ = velocity;
}