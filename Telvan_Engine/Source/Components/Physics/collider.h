/*************************************************************************************************************************************
* Title: collider.h
* Author: Jennifer Assid
* Date of Creation: 16 April 2023
*
**************************************************************************************************************************************/
#pragma once
#ifndef COLLIDER_CLASS_H
#define COLLIDER_CLASS_H

#include <functional>
#include <glm/glm.hpp>

#include "components.h"

#include "shader.h"

class Collider;
class Circle;
class AABB;

// Default trigger equations
inline bool on_enter_default(Collider&, Collider&) { return true; }
inline bool on_exit_default(Collider&, Collider&) { return false; }
inline bool while_triggered_default(Collider&, Collider&) { return true; }

class Collider : public Component
{
	/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:
	enum class Collider_Type
	{
		col_Undefined = -1,
		col_Circle,
		col_AABB
	};

	using Trigger_Function = std::function<bool(Collider&, Collider&)>;

	/************************************************************ VARIABLES **************************************************************/
private:

protected:
	// Collider Information
	Collider_Type collider_type_;
	glm::vec2 offset_;

	float stiffness_value_;

	// Trigger Logistics
	bool is_trigger_;
	bool triggered_;
	Trigger_Function on_enter_;
	Trigger_Function on_exit_;
	Trigger_Function while_triggered_;

	// Debug Drawing Information
	glm::vec4 outline_color_;
	Shader line_shader_;
	unsigned int collider_outline_VAO_;

	glm::vec4 point_color_;
	unsigned int point_VAO_;

	glm::vec4 vec_color_;
	unsigned int vec_VAO_;

	std::vector<glm::vec2> collision_points_;

public:

	/************************************************************ FUNCTIONS **************************************************************/
private:

	// Focused Collision Detection
	// Circle-Circle
	bool static_circle_circle_check(Circle& static_a, Circle& static_b, float dT);
	bool static_dynamic_circle_circle_check(Circle& dynamic, Circle& static_circ, float dT);

	// Circle-AABB
	bool static_circle_aabb_check(Circle& static_circ, AABB& static_aabb, float dT);
	bool static_circle_dynamic_aabb_check(Circle& static_circ, AABB& dynamic_aabb, float dT);
	bool dynamic_circle_static_aabb_check(Circle& dynamic_circ, AABB& static_aabb, float dT);

	// AABB-AABB
	bool static_aabb_aabb_check(AABB& a, AABB& b, float dT);
	bool static_dynamic_aabb_aabb_check(AABB& dynamic_aabb, AABB& static_aabb, float dT);

	// Focused Collision Resolution
	// Circle-Circle
	void static_circle_circle_response(Circle& static_a, Circle& static_b, float dT);
	void dynamic_static_circle_circle_response(Circle& dynamic, Circle& static_circ, float dT);
	void dynamic_circle_circle_response(Circle& dynamic_a, Circle& dynamic_b, float dT);

	// Circle-AABB
	void static_circle_aabb_response(Circle& static_circ, AABB& static_aabb, float dT);
	void static_circle_dynamic_aabb_response(Circle& static_circ, AABB& dynamic_aabb, float dT);
	void dynamic_circle_static_aabb_response(Circle& dynamic_circ, AABB& static_aabb, float dT);
	void dynamic_circle_aabb_response(Circle& dynamic_circ, AABB& dynamic_aabb, float dT);

	// AABB-AABB
	void static_aabb_aabb_response(AABB& a, AABB& b, float dT);
	void dynamic_static_aabb_aabb_response(AABB& dynamic_aabb, AABB& static_aabb, float dT);
	void dynamic_aabb_aabb_response(AABB& a, AABB& b, float dT);

	// Utility Functions
	bool circle_circle_overlap(glm::vec2 a_pos,
		float a_rad,
		glm::vec2 b_pos,
		float b_rad);

	bool aabb_circle_overlap(glm::vec2 aabb_pos,
		glm::vec2 aabb_halfs,
		glm::vec2 circ_pos,
		float radius);

	bool aabb_aabb_overlap(glm::vec2 a_pos,
		glm::vec2 a_halfs,
		glm::vec2 b_pos,
		glm::vec2 b_halfs);

protected:
	// Collision Detection
	bool circle_circle_check(Circle& a, Circle& b, float dT);
	bool circle_aabb_check(Circle& a, AABB& b, float dT);
	bool aabb_aabb_check(AABB& a, AABB& b, float dT);

	// Collision Resolutuion
	void circle_circle_response(Circle& a, Circle& b, float dT);
	void circle_aabb_response(Circle& a, AABB& b, float dT);
	void aabb_aabb_response(AABB& a, AABB& b, float dT);

	// Render
	void render_collision_points();
	void render_vectors();

	void init_point();
	void init_vec();

public:
	// Base Component Functions
	Collider() : Component(Component_Type::ct_Collider),
		collider_type_(Collider_Type::col_Undefined),
		offset_(glm::vec2(0.0f)),
		is_trigger_(false),
		triggered_(false),
		on_enter_(on_enter_default),
		on_exit_(on_exit_default),
		while_triggered_(while_triggered_default),
		outline_color_(glm::vec4(1.0f)),
		point_color_(glm::vec4(1.0f)),
		vec_color_(glm::vec4(1.0f))
	{}

	Collider(Collider_Type type,
		glm::vec2 offset = glm::vec2(0.0f)) : Component(Component_Type::ct_Collider),
		collider_type_(type),
		offset_(offset),
		is_trigger_(false),
		triggered_(false),
		on_enter_(on_enter_default),
		on_exit_(on_exit_default),
		while_triggered_(while_triggered_default),
		outline_color_(glm::vec4(1.0f)),
		point_color_(glm::vec4(1.0f)),
		vec_color_(glm::vec4(1.0f))
	{}

	virtual ~Collider() {}

	virtual Component* Clone() override { return nullptr; };

	virtual void Start() override {}
	virtual void Render() override {}

	virtual void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) override {}
	virtual void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader) override {}

	// Collision Functions
	virtual bool Collision_Detection(Collider& other, float dT) { return false; }
	virtual void Collision_Response(Collider& other, float dT) {}

	// Getter & Setter Functions
	inline Collider_Type Get_Collider_Type() const { return collider_type_; }
	inline glm::vec2 Get_Offset() const { return offset_; }
	inline float Get_Stiffness_Value() const { return stiffness_value_; }
	inline bool Get_Is_Trigger() const { return is_trigger_; }
	inline bool Get_Triggered() const { return triggered_; }
	inline glm::vec4 Get_Outline_Color() const { return outline_color_; }
	inline glm::vec4 Get_Point_Color() const { return point_color_; }
	inline glm::vec4 Get_Vec_Color() const { return vec_color_; }

	inline void Set_Offset(glm::vec2 offset) { offset_ = offset; }
	inline void Set_StiffnessValue(float stiffness_value) { stiffness_value_ = stiffness_value; }
	inline void Set_Is_Trigger(bool is_trigger) { is_trigger_ = is_trigger; }
	inline void Set_Outline_Color(glm::vec4 color) { outline_color_ = color; }
	inline void Set_Point_Color(glm::vec4 color) { point_color_ = color; }
	inline void Set_Vec_Color(glm::vec4 color) { vec_color_ = color; }

	void Set_Triggered(bool triggered, Collider& other)
	{
		if (is_trigger_ == false) return;

		if (triggered_ == false && triggered == true)
			triggered_ = on_enter_(*this, other);
		else if (triggered_ == true && triggered == true)
			triggered_ = while_triggered_(*this, other);
		else if (triggered_ == true && triggered == false)
			triggered_ = on_exit_(*this, other);
	}

	inline void Set_On_Enter(Trigger_Function on_enter) { on_enter_ = (on_enter == nullptr) ? on_enter_default : on_enter; }
	inline void Set_On_Exit(Trigger_Function on_exit) { on_exit_ = (on_exit == nullptr) ? on_exit_default : on_exit; ; }
	inline void Set_While_Triggered(Trigger_Function while_triggered) { while_triggered_ = (while_triggered == nullptr) ? while_triggered_default : while_triggered; ; }
};

class Circle : public Collider
{
	/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

	/************************************************************ VARIABLES **************************************************************/
private:
	float radius_;
	unsigned int segments_;

protected:
public:

	/************************************************************ FUNCTIONS **************************************************************/
private:
	void initialize_circle_outline();

protected:
public:
	Circle() : Collider(Collider_Type::col_Circle),
		radius_(10.0f),
		segments_(36)
	{}

	~Circle() override {}

	Component* Clone() override;

	void Start() override;

	void Render() override;

	void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) override;
	void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader) override;

	bool Collision_Detection(Collider& other) override;
	void Collision_Response(Collider& other) override;

	inline float Get_Radius() const { return radius_; }

	inline void Set_Radius(float radius) { radius_ = radius; }
};

class AABB : public Collider
{
	/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

	/************************************************************ VARIABLES **************************************************************/
private:
	glm::vec2 half_length_;

protected:
public:

	/************************************************************ FUNCTIONS **************************************************************/
private:
	void initialize_square_outline();

protected:
public:
	AABB() : Collider(Collider_Type::col_AABB),
		half_length_(glm::vec2(10.0f, 10.0f))
	{}

	~AABB() override {};

	Component* Clone() override;

	void Start() override;

	void Render() override;

	void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) override;
	void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader) override;

	bool Collision_Detection(Collider& other) override;
	void Collision_Response(Collider& other) override;

	inline glm::vec2 Get_Half_Length() const { return half_length_; }

	inline void Set_Half_Length(glm::vec2 half_length) { half_length_ = half_length; }
};

#endif // !COLLIDER_CLASS_H