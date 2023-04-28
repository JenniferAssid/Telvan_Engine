/*************************************************************************************************************************************
* Title: rigid_body.h
* Author: Jennifer Assid
* Date of Creation: 17 April 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef RIGID_BODY_CLASS_H
#define RIGID_BODY_CLASS_H

#include <glm/glm.hpp>

#include "components.h"

class Rigid_Body : public Component
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private: 
	glm::vec2 direction_;

	float current_velocity_;
	float max_velocity_;

	glm::vec2 total_force_;

	float mass_;

protected:

public:

/************************************************************ FUNCTIONS **************************************************************/
private:

protected:

public:
	Rigid_Body() : Component(Component_Type::ct_Rigid_Body),
		direction_(glm::vec2(0.0f)),
		current_velocity_(0.0f),
		max_velocity_(5.0f),
		total_force_(glm::vec2(0.0f)),
		mass_(1.0f)
		{}

	Component* Clone() override;

	void Pre_Update(float dT) override;

	void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) override;
	void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader) override;

	inline glm::vec2 Get_Direction() const { return direction_; }
	inline float Get_Current_Velocity() const { return current_velocity_; }
	inline float Get_Max_Velocity() const { return max_velocity_; }
	inline glm::vec2 Get_Total_Force() const { return total_force_; }
	inline float Get_Mass() const { return mass_; }

	void Add_Force(glm::vec2 force);
	void Set_Current_Velocity(glm::vec2 velocity);
	inline void Set_Mass(float mass) { mass_ = mass; }
};

#endif // !RIGID_BODY_CLASS_H
