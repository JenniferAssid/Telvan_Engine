#pragma once

#include "camera.h"
#include "error_logging.h"
#include "transform.h"
#include "engine.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void Camera::Start()
{
	if (parent_ == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message(
			"Missing parent pointer",
			Error_Logging::Message_Level::ot_Warning,
			"Camera",
			"Start"
		);
		return;
	}

	Transform* transform = parent_->Get_Component<Transform>(Component_Type::ct_Transform);
	if (transform == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message(
			"Parent missing transform component",
			Error_Logging::Message_Level::ot_Warning,
			"Camera",
			"Start"
		);
		return;
	}

	camera_position_ = transform->Get_Translation();
}

void Camera::Update(float dT)
{
	Transform* desired_position = parent_->Get_Component<Transform>(Component_Type::ct_Transform);
	if (desired_position == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message(
			"Parent missing transform component",
			Error_Logging::Message_Level::ot_Warning,
			"Camera",
			"Update"
		);
		return;
	}

	glm::vec2 direction = desired_position->Get_Translation() - camera_position_;
	glm::normalize(direction);

	camera_position_ += direction * speed_ * dT;

	glm::vec3 position = glm::vec3(camera_position_, desired_position->Get_Z_Sorting_Value());
	position.y *= -1.0f;
	position.x -= Engine::Get_Instance()->Get_Width() * 0.5f;
	position.y -= Engine::Get_Instance()->Get_Height() * 0.5f;

	view_matrix_ = glm::lookAt(position,
		position + glm::vec3(0.0f, 0.0f, -zoom_),
		glm::vec3(0.0f, 1.0f, 0.0f));
	/*view_matrix_ = glm::rotate(view_matrix_, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view_matrix_ = glm::rotate(view_matrix_, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));*/
}

void Camera::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values)
{
	writer.Key("camera");
	writer.StartObject();

	writer.Key("zoom");
	writer.Double(zoom_);
	
	writer.Key("speed");
	writer.Double(speed_);

	writer.EndObject();
}

void Camera::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader)
{
	if (reader.HasMember("camera") == false) return;

	if (reader["camera"].GetObject().HasMember("zoom") &&
		reader["camera"]["zoom"].IsDouble())
		zoom_ = (float)reader["camera"]["zoom"].GetDouble();

	if (reader["camera"].GetObject().HasMember("speed") &&
		reader["camera"]["speed"].IsDouble())
		zoom_ = (float)reader["camera"]["speed"].GetDouble();
}