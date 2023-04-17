#pragma once
#ifndef COLLIDER_CLASS_H
#define COLLIDER_CLASS_H

#include "components.h"

#include "shader.h"

class Collider : public Component
{
public:
	enum class Collider_Type
	{
		col_Undefined = -1,
		col_Circle,
		col_AABB
	};

protected:
	Collider_Type collider_type_;
	glm::vec2 offset_;

	glm::vec4 color_;

	Shader shader_;

	unsigned int line_VAO_;

protected:
	void initialize_square_outline();
	

public:
	Collider() : Component(Component_Type::ct_Collider),
		collider_type_(Collider_Type::col_Undefined),
		offset_(glm::vec2(0.0f)),
		color_(glm::vec4(1.0f))
	{}

	Collider(Collider_Type type, 
		glm::vec2 offset = glm::vec2(0.0f)) : Component(Component_Type::ct_Collider),
		collider_type_(type),
		offset_(offset),
		color_(glm::vec4(1.0f))
	{}

	virtual ~Collider() {}

	virtual void Start() override {}

	virtual void Render() override {}
	virtual bool Collision_Detection(Collider& other) { return false; }

	inline Collider_Type Get_Collider_Type() const { return collider_type_; }
	inline glm::vec2 Get_Offset() const { return offset_; }
	inline glm::vec4 Get_Color() const { return color_; }

	inline void Set_Offset(glm::vec2 offset) { offset_ = offset; }
	inline void Get_Color(glm::vec4 color) { color_ = color; }
};

class Circle : public Collider
{
private:
	float radius_;
	unsigned int segments_;

private:
	void initialize_circle_outline();

public:
	Circle() : Collider(Collider_Type::col_Circle),
	radius_(10.0f),
	segments_(36)
	{}

	~Circle() override {}

	void Start() override;

	void Render() override;

	//bool Collision_Detection(Collider& other) override;

	inline float Get_Radius() const { return radius_; }

	inline void Set_Radius(float radius) { radius_ = radius; }
};

class AABB : public Collider
{
private:
	glm::vec2 half_length_;
	
private:
	void initialize_square_outline();

public:
	AABB() : Collider(Collider_Type::col_AABB),
		half_length_(glm::vec2(10.0f, 10.0f))
	{}

	~AABB() override {};

	void Start() override;

	void Render() override;

	//bool Collision_Detection(Collider& other) override;

	inline glm::vec2 Get_Half_Length() const { return half_length_; }

	inline void Set_Half_Length(glm::vec2 half_length) { half_length_ = half_length; }
};

#endif // !COLLIDER_CLASS_H
