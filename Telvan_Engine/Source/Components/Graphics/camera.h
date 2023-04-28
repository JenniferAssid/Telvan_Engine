/*************************************************************************************************************************************
* Title: camera.h
* Author: Jennifer Assid
* Date of Creation: 23 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>

#include "components.h"

class Entity;

class Camera : Component
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
	float zoom_;
	float speed_;
	glm::vec2 camera_position_;
	glm::mat4 view_matrix_;

protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
	Camera() : Component(Component_Type::ct_Camera),
		zoom_(1.0f), speed_(2.0f), camera_position_(glm::vec2(1.0f)), view_matrix_(glm::mat4(1.0f))
	{}

	Component* Clone() override;

	void Start() override;

	void Update(float dT) override;

	void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) override;
	void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer) override;

	inline void Set_Zoom(float zoom) { zoom_ = zoom; }
	inline void Set_Speed(float speed) { speed_ = speed; }
	inline void Set_Camera_Position(glm::vec2 position) { camera_position_ = position; }

	inline float Get_Zoom() { return zoom_; }
	inline float Get_Speed() { return speed_; }
	inline glm::vec2 Get_Camera_Position() { return camera_position_; }
	inline glm::mat4 Get_Matrix() { return view_matrix_; }
};

#endif // !CAMERA_CLASS_H
