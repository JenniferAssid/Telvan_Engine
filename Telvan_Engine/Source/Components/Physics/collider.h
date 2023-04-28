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

	// Trigger Logistics
	bool is_trigger_;
	bool triggered_;
	Trigger_Function on_enter_;
	Trigger_Function on_exit_;
	Trigger_Function while_triggered_;

	// Debug Drawing Information
	glm::vec4 color_;
	Shader shader_;
	unsigned int line_VAO_;

public:

/************************************************************ FUNCTIONS **************************************************************/
private:
	// Focused Collision Detection
	// Circle-Circle
	bool static_circle_circle_check(Circle& static_a, Circle& static_b);
	bool static_dynamic_circle_circle_check(Circle& dynamic, Circle& static_circ);

	// Circle-AABB
	bool static_circle_aabb_check(Circle& static_circ, AABB& static_aabb);
	bool static_circle_dynamic_aabb_check(Circle& static_circ, AABB& dynamic_aabb);
	bool dynamic_circle_static_aabb_check(Circle& dynamic_circ, AABB& static_aabb);

	// AABB-AABB
	bool static_aabb_aabb_check(AABB& a, AABB& b);
	bool static_dynamic_aabb_aabb_check(AABB& dynamic_aabb, AABB& static_aabb);

	// Focused Collision Resolution
	// Circle-Circle
	void static_circle_circle_response(Circle& static_a, Circle& static_b);
	void dynamic_static_circle_circle_response(Circle& dynamic, Circle& static_circ);
	void dynamic_circle_circle_response(Circle& dynamic_a, Circle& dynamic_b);

	// Circle-AABB
	void static_circle_aabb_response(Circle& static_circ, AABB& static_aabb);
	void static_circle_dynamic_aabb_response(Circle& static_circ, AABB& dynamic_aabb);
	void dynamic_circle_static_aabb_response(Circle& dynamic_circ, AABB& static_aabb);
	void dynamic_circle_aabb_response(Circle& dynamic_circ, AABB& dynamic_aabb);

	// AABB-AABB
	void static_aabb_aabb_response(AABB& a, AABB& b);
	void dynamic_static_aabb_aabb_response(AABB& dynamic_aabb, AABB& static_aabb);
	void dynamic_aabb_aabb_response(AABB& a, AABB& b);

protected:
	// Collision Detection
	bool circle_circle_check(Circle& a, Circle& b);
	bool circle_aabb_check(Circle& a, AABB& b);
	bool aabb_aabb_check(AABB& a, AABB& b);

	// Collision Resolutuion
	void circle_circle_response(Circle& a, Circle& b);
	void circle_aabb_response(Circle& a, AABB& b);
	void aabb_aabb_response(AABB& a, AABB& b);

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
		color_(glm::vec4(1.0f))
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
		color_(glm::vec4(1.0f))
	{}

	virtual ~Collider() {}

	virtual Component* Clone() override { return nullptr; };

	virtual void Start() override {}
	virtual void Render() override {}

	virtual void Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values = true) override {}
	virtual void Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader) override {}

	// Collision Functions
	virtual bool Collision_Detection(Collider& other) { return false; }
	virtual void Collision_Response(Collider& other) {}

	// Getter & Setter Functions
	inline Collider_Type Get_Collider_Type() const { return collider_type_; }
	inline glm::vec2 Get_Offset() const { return offset_; }
	inline bool Get_Is_Trigger() const { return is_trigger_; }
	inline bool Get_Triggered() const { return triggered_; }
	inline glm::vec4 Get_Color() const { return color_; }

	inline void Set_Offset(glm::vec2 offset) { offset_ = offset; }
	inline void Set_Color(glm::vec4 color) { color_ = color; }
	inline void Set_Is_Trigger(bool is_trigger) { is_trigger_ = is_trigger; }

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

	inline void Set_On_Enter(Trigger_Function on_enter) { on_enter_ = (on_enter == nullptr) ? on_enter_default: on_enter; }
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
