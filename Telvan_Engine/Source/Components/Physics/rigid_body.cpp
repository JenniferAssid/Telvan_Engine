/*************************************************************************************************************************************
* Title: rigid_body.cpp
* Author: Jennifer Assid
* Date of Creation: 17 April 2023
*
**************************************************************************************************************************************/

#include "rigid_body.h"
#include "transform.h"

Component* Rigid_Body::Clone()
{
	Rigid_Body* rb = new Rigid_Body();
	rb->direction_ = direction_;
	rb->max_velocity_ = max_velocity_;
	rb->mass_ = mass_;

	return (Component*)rb;
}

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

void Rigid_Body::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values)
{
	writer.Key("rigid_body");
	writer.StartObject();

	writer.Key("direction");
	writer.StartArray();
	writer.Double(direction_.x);
	writer.Double(direction_.y);
	writer.EndArray();

	writer.Key("max_velocity");
	writer.Double(max_velocity_);

	writer.Key("mass");
	writer.Double(mass_);

	writer.EndObject();
}

void Rigid_Body::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader)
{
	if (reader.HasMember("rigid_body") == false) return;

	if (reader["rigid_body"].GetObject().HasMember("direction") &&
		reader["rigid_body"]["direction"].IsArray())
	{
		const rapidjson::Value& direction = reader["rigid_body"]["direction"];

		direction_ = glm::vec2((float)direction[0].GetDouble(),
			(float)direction[1].GetDouble());
	}

	if (reader["rigid_body"].GetObject().HasMember("max_velocity") &&
		reader["rigid_body"]["max_velocity"].IsDouble())
		max_velocity_ = reader["rigid_body"]["max_velocity"].GetDouble();

	if (reader["rigid_body"].GetObject().HasMember("mass") &&
		reader["rigid_body"]["mass"].IsDouble())
		mass_ = reader["rigid_body"]["mass"].GetDouble();
}

void Rigid_Body::Add_Force(glm::vec2 force)
{
	total_force_ += force;
}

void Rigid_Body::Set_Current_Velocity(glm::vec2 velocity)
{
	total_force_ = velocity;
}