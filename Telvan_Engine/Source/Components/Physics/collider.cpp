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

#include "collider_manager.h"

/************************************************* COLLIDER: Private Functions *******************************************************/

// Focused Collision Detection

// Circle-Circle
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

	glm::vec2 closest_point = glm::closestPointOnLine(static_trans->Get_Translation() + static_circ.Get_Offset(),
		dynamic_trans->Get_Translation() + dynamic.Get_Offset(),
		dynamic_trans->Get_Translation() + dynamic.Get_Offset() + dynamic_rb->Get_Direction() * dynamic_rb->Get_Current_Velocity());

	glm::vec2 distance = static_trans->Get_Translation() + static_circ.Get_Offset() - closest_point;

	if (glm::length(distance) < dynamic.Get_Radius() + static_circ.Get_Radius())
		return true;

	return false;
}

// Circle-AABB
bool Collider::static_circle_aabb_check(Circle& static_circ, AABB& static_aabb)
{
	Transform* circ_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (circ_trans == nullptr) return false;

	Transform* aabb_trans = static_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (aabb_trans == nullptr) return false;

	glm::vec2 aabb_pos = aabb_trans->Get_Translation();
	glm::vec2 aabb_half_lengths = static_aabb.Get_Half_Length();

	glm::vec2 min = aabb_pos - aabb_half_lengths;
	glm::vec2 max = aabb_pos + aabb_half_lengths;

	float circ_radius = static_circ.Get_Radius();

	glm::vec2 distance = circ_trans->Get_Translation() - min;
	if (glm::length(distance) <= circ_radius) return true;

	distance = circ_trans->Get_Translation() - glm::vec2(min.x, max.y);
	if (glm::length(distance) <= circ_radius) return true;

	distance = circ_trans->Get_Translation() - glm::vec2(max.x, min.y);
	if (glm::length(distance) <= circ_radius) return true;

	distance = circ_trans->Get_Translation() - max;
	if (glm::length(distance) <= circ_radius) return true;

	distance = circ_trans->Get_Translation() - aabb_trans->Get_Translation();
	if (glm::length(distance) <= circ_radius) return true;

	return false;
}

bool Collider::static_circle_dynamic_aabb_check(Circle& static_circ, AABB& dynamic_aabb)
{
	Rigid_Body* dynamic_rb = dynamic_aabb.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return false;

	Transform* static_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return false;

	Transform* dynamic_trans = dynamic_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return false;

	glm::vec2 direction = dynamic_rb->Get_Direction();

	glm::vec2 destination_point = dynamic_trans->Get_Translation() + dynamic_aabb.Get_Offset()
		+ dynamic_rb->Get_Direction() * dynamic_rb->Get_Current_Velocity();

	glm::vec2 lower_left = dynamic_trans->Get_Translation() + dynamic_aabb.Get_Offset() - dynamic_aabb.Get_Half_Length();
	glm::vec2 upper_right = dynamic_trans->Get_Translation() + dynamic_aabb.Get_Offset() + dynamic_aabb.Get_Half_Length();

	glm::vec2 lower_right(upper_right.x, lower_left.y);
	glm::vec2 upper_left(lower_left.x, upper_right.y);

	if (glm::length((static_trans->Get_Translation() + static_circ.Get_Offset()) - lower_left) <= static_circ.Get_Radius())
		return true;

	if (glm::length((static_trans->Get_Translation() + static_circ.Get_Offset()) - upper_right) <= static_circ.Get_Radius())
		return true;

	if (glm::length((static_trans->Get_Translation() + static_circ.Get_Offset()) - lower_right) <= static_circ.Get_Radius())
		return true;

	if (glm::length((static_trans->Get_Translation() + static_circ.Get_Offset()) - upper_left) <= static_circ.Get_Radius())
		return true;

	glm::vec2 distance = destination_point - (static_trans->Get_Translation() + static_circ.Get_Offset());

	glm::vec2 aabb_vec(glm::normalize(distance).x * dynamic_aabb.Get_Half_Length().x,
		glm::normalize(distance).y * dynamic_aabb.Get_Half_Length().y);
	
	if (glm::length(distance) <= glm::length(aabb_vec) + static_circ.Get_Radius())
		return true;

	return false;
}

bool Collider::dynamic_circle_static_aabb_check(Circle& dynamic_circ, AABB& static_aabb)
{
	Rigid_Body* dynamic_rb = dynamic_circ.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return false;

	Transform* static_trans = static_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return false;

	Transform* dynamic_trans = dynamic_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return false;

	glm::vec2 direction = dynamic_rb->Get_Direction();

	glm::vec2 destination_point = dynamic_trans->Get_Translation() + dynamic_circ.Get_Offset()
		+ dynamic_rb->Get_Direction() * dynamic_rb->Get_Current_Velocity();

	glm::vec2 lower_left = dynamic_trans->Get_Translation() + static_aabb.Get_Offset() - static_aabb.Get_Half_Length();
	glm::vec2 upper_right = dynamic_trans->Get_Translation() + static_aabb.Get_Offset() + static_aabb.Get_Half_Length();

	glm::vec2 lower_right(upper_right.x, lower_left.y);
	glm::vec2 upper_left(lower_left.x, upper_right.y);

	glm::vec2 static_position = static_trans->Get_Translation() + static_aabb.Get_Offset();

	if (glm::length(static_position - lower_left) <= dynamic_circ.Get_Radius())
		return true;

	if (glm::length(static_position - upper_right) <= dynamic_circ.Get_Radius())
		return true;

	if (glm::length(static_position - lower_right) <= dynamic_circ.Get_Radius())
		return true;

	if (glm::length(static_position - upper_left) <= dynamic_circ.Get_Radius())
		return true;

	glm::vec2 distance = destination_point - static_position;

	glm::vec2 aabb_vec(glm::normalize(distance).x * static_aabb.Get_Half_Length().x,
		glm::normalize(distance).y * static_aabb.Get_Half_Length().y);

	if (glm::length(distance) <= glm::length(aabb_vec) + dynamic_circ.Get_Radius())
		return true;

	return false;
}

// AABB-AABB
bool Collider::static_aabb_aabb_check(AABB& a, AABB& b)
{
	Transform* a_trans = a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return false;

	Transform* b_trans = b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return false;

	glm::vec2 a_pos = a_trans->Get_Translation() + a.Get_Offset();
	glm::vec2 a_halfs = a.Get_Half_Length();

	glm::vec2 b_pos = b_trans->Get_Translation() + b.Get_Offset();
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

bool Collider::static_dynamic_aabb_aabb_check(AABB& dynamic_aabb, AABB& static_aabb)
{
	Rigid_Body* dynamic_rb = dynamic_aabb.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return false;

	Transform* static_trans = static_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return false;

	Transform* dynamic_trans = dynamic_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return false;

	glm::vec2 direction = glm::normalize(dynamic_rb->Get_Direction());

	glm::vec2 destination_point = dynamic_trans->Get_Translation() + dynamic_aabb.Get_Offset()
		+ dynamic_rb->Get_Direction() * dynamic_rb->Get_Current_Velocity();

	glm::vec2 lower_left = static_trans->Get_Translation() + static_aabb.Get_Offset() - static_aabb.Get_Half_Length();
	glm::vec2 upper_right = static_trans->Get_Translation() + static_aabb.Get_Offset() + static_aabb.Get_Half_Length();

	// Upper Left Corner Intersection
	if ((destination_point.x - dynamic_aabb.Get_Half_Length().x <= upper_right.x)
		&& (destination_point.y + dynamic_aabb.Get_Half_Length().y <= upper_right.y)
		&& (destination_point.x - dynamic_aabb.Get_Half_Length().x >= lower_left.x)
		&& (destination_point.y + dynamic_aabb.Get_Half_Length().y >= lower_left.y))
		return true;

	// Upper Right Corner Intersection
	if ((destination_point.x + dynamic_aabb.Get_Half_Length().x <= upper_right.x)
		&& (destination_point.y + dynamic_aabb.Get_Half_Length().y <= upper_right.y)
		&& (destination_point.x + dynamic_aabb.Get_Half_Length().x >= lower_left.x)
		&& (destination_point.y + dynamic_aabb.Get_Half_Length().y >= lower_left.y))
		return true;

	// Lower Left Corner Intersection
	if ((destination_point.x - dynamic_aabb.Get_Half_Length().x <= upper_right.x)
		&& (destination_point.y - dynamic_aabb.Get_Half_Length().y <= upper_right.y)
		&& (destination_point.x - dynamic_aabb.Get_Half_Length().x >= lower_left.x)
		&& (destination_point.y - dynamic_aabb.Get_Half_Length().y >= lower_left.y))
		return true;

	// Lower Right Corner Intersection
	if ((destination_point.x + dynamic_aabb.Get_Half_Length().x <= upper_right.x)
		&& (destination_point.y - dynamic_aabb.Get_Half_Length().y <= upper_right.y)
		&& (destination_point.x + dynamic_aabb.Get_Half_Length().x >= lower_left.x)
		&& (destination_point.y - dynamic_aabb.Get_Half_Length().y >= lower_left.y))
		return true;

	return false;
}

// Focused Collision Resolution

// Circle-Circle
void Collider::static_circle_circle_response(Circle& static_a, Circle& static_b)
{
	Transform* a_trans = static_a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return;

	Transform* b_trans = static_b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return;

	glm::vec2 midpoint = ((b_trans->Get_Translation() + static_b.Get_Offset()) 
		+ (a_trans->Get_Translation() + static_a.Get_Offset())) / 2.0f;

	a_trans->Set_Translation((midpoint - static_a.Get_Offset())
		+ static_a.Get_Radius() * glm::normalize((a_trans->Get_Translation() + static_a.Get_Offset()) -
			(b_trans->Get_Translation() + static_b.Get_Offset())));
	b_trans->Set_Translation((midpoint - static_a.Get_Offset())
		+ static_a.Get_Radius() * glm::normalize((b_trans->Get_Translation() + static_b.Get_Offset()) - 
			(a_trans->Get_Translation() + static_a.Get_Offset())));
}

void Collider::dynamic_static_circle_circle_response(Circle& dynamic, Circle& static_circ)
{
	Transform* dynamic_trans = dynamic.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return;

	Transform* static_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return;

	Rigid_Body* dynamic_rb = dynamic.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return;

	glm::vec2 n = (dynamic_trans->Get_Translation() + dynamic.Get_Offset())
		- (static_trans->Get_Translation() + static_circ.Get_Offset());
	n = glm::normalize(n);

	float p = (2.0f * glm::dot(dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction(), n))
		/ (dynamic_rb->Get_Mass() + dynamic_rb->Get_Mass());

	glm::vec2 w = dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction()
		- p * (dynamic_rb->Get_Mass() * n - dynamic_rb->Get_Mass() * n);

	glm::vec2 intersection = ((static_trans->Get_Translation() + static_circ.Get_Offset())
		- (dynamic_trans->Get_Translation() + dynamic.Get_Offset()));
	float intersect_length = std::max((dynamic.Get_Radius() + static_circ.Get_Radius()) - glm::length(intersection),
		0.0f);
	intersect_length += 5.0f;

	intersection = glm::normalize(intersection) * -intersect_length;

	dynamic_rb->Set_Current_Velocity(w + intersection);
}

void Collider::dynamic_circle_circle_response(Circle& dynamic_a, Circle& dynamic_b)
{
	Transform* a_trans = dynamic_a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return;

	Transform* b_trans = dynamic_b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return;

	Rigid_Body* a_rb = dynamic_a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (a_rb == nullptr) return;

	Rigid_Body* b_rb = dynamic_b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (b_rb == nullptr) return;

	glm::vec2 distance = (b_trans->Get_Translation() + dynamic_b.Get_Offset())
		- (a_trans->Get_Translation() + dynamic_a.Get_Offset());
	distance = glm::normalize(distance);

	glm::vec2 a_v = a_rb->Get_Current_Velocity() * a_rb->Get_Direction();
	glm::vec2 b_v = b_rb->Get_Current_Velocity() * b_rb->Get_Direction();

	float p = (2.0f * (glm::dot(a_v, distance) - glm::dot(b_v, distance))) / (a_rb->Get_Mass() + b_rb->Get_Mass());

	glm::vec2 intersection = ((b_trans->Get_Translation() + dynamic_b.Get_Offset())
		- (a_trans->Get_Translation() + dynamic_a.Get_Offset()));

	glm::vec2 a_w = a_v - p * a_rb->Get_Mass() * distance;
	glm::vec2 b_w = b_v + p * b_rb->Get_Mass() * distance;

	a_rb->Set_Current_Velocity(a_w - intersection);
	b_rb->Set_Current_Velocity(b_w + intersection);
}

// Circle-AABB
void Collider::static_circle_aabb_response(Circle& static_circ, AABB& static_aabb)
{
	Transform* circ_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (circ_trans == nullptr) return;

	Transform* aabb_trans = static_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (aabb_trans == nullptr) return;

	// AABB->Circle
	glm::vec2 distance = (circ_trans->Get_Translation() + static_circ.Get_Offset())
		- (aabb_trans->Get_Translation() + static_aabb.Get_Offset());

	glm::vec2 midpoint = glm::normalize(distance) * static_aabb.Get_Half_Length();

	circ_trans->Set_Translation((midpoint - static_circ.Get_Offset())
		+ static_circ.Get_Radius() * glm::normalize(distance));

	aabb_trans->Set_Translation((midpoint - static_aabb.Get_Offset())
		+ static_aabb.Get_Half_Length() * -glm::normalize(distance));
}

void Collider::static_circle_dynamic_aabb_response(Circle& static_circ, AABB& dynamic_aabb)
{
	Rigid_Body* dynamic_rb = dynamic_aabb.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return;

	Transform* circ_trans = static_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (circ_trans == nullptr) return;

	Transform* aabb_trans = dynamic_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (aabb_trans == nullptr) return;

	glm::vec2 n = (aabb_trans->Get_Translation() + dynamic_aabb.Get_Offset())
		- (circ_trans->Get_Translation() + static_circ.Get_Offset());
	n = glm::normalize(n);

	float p = (2.0f * glm::dot(dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction(), n))
		/ (dynamic_rb->Get_Mass() + dynamic_rb->Get_Mass());

	glm::vec2 w = dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction()
		- p * (dynamic_rb->Get_Mass() * n - dynamic_rb->Get_Mass() * n);

	// AABB->Circle
	glm::vec2 intersection = (circ_trans->Get_Translation() + static_circ.Get_Offset())
		- (aabb_trans->Get_Translation() + dynamic_aabb.Get_Offset());

	float intersection_length = std::max(static_circ.Get_Radius()
		+ glm::length(glm::normalize(intersection) * dynamic_aabb.Get_Half_Length())
		- glm::length(intersection),
		0.0f);
	intersection_length += 5.0f;

	intersection = glm::normalize(intersection) * -intersection_length;

	dynamic_rb->Set_Current_Velocity(w + intersection);
}

void Collider::dynamic_circle_static_aabb_response(Circle& dynamic_circ, AABB& static_aabb)
{
	Rigid_Body* dynamic_rb = dynamic_circ.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return;

	Transform* circ_trans = dynamic_circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (circ_trans == nullptr) return;

	Transform* aabb_trans = static_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (aabb_trans == nullptr) return;

	glm::vec2 n = (circ_trans->Get_Translation() + dynamic_circ.Get_Offset())
		- (aabb_trans->Get_Translation() + static_aabb.Get_Offset());
	n = glm::normalize(n);

	float p = (2.0f * glm::dot(dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction(), n))
		/ (dynamic_rb->Get_Mass() + dynamic_rb->Get_Mass());

	glm::vec2 w = dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction()
		- p * (dynamic_rb->Get_Mass() * n - dynamic_rb->Get_Mass() * n);

	glm::vec2 intersection = (circ_trans->Get_Translation() + dynamic_circ.Get_Offset())
		- (aabb_trans->Get_Translation() + static_aabb.Get_Offset());

	glm::vec2 lower_left = aabb_trans->Get_Translation() + static_aabb.Get_Offset() - static_aabb.Get_Half_Length();
	glm::vec2 upper_right = aabb_trans->Get_Translation() + static_aabb.Get_Offset() + static_aabb.Get_Half_Length();

	glm::vec2 lower_right(upper_right.x, lower_left.y);
	glm::vec2 upper_left(lower_left.x, upper_right.y);

	glm::vec2 possible_intersection;

	if (intersection.x >= 0.0f)
	{
		if (intersection.y >= 0.0f)
			possible_intersection = circ_trans->Get_Translation() + dynamic_circ.Get_Offset() - upper_right;
		else
			possible_intersection = circ_trans->Get_Translation() + dynamic_circ.Get_Offset() - lower_right;
	}
	else
	{
		if (intersection.y >= 0.0f)
			possible_intersection = circ_trans->Get_Translation() + dynamic_circ.Get_Offset() - upper_left;
		else
			possible_intersection = circ_trans->Get_Translation() + dynamic_circ.Get_Offset() - lower_left;
	}

	if (glm::length(intersection) > glm::length(possible_intersection))
		intersection = possible_intersection;

	float intersection_length = std::max(dynamic_circ.Get_Radius()
		+ glm::length(glm::vec2(glm::normalize(intersection).x * static_aabb.Get_Half_Length().x,
			glm::normalize(intersection).y * static_aabb.Get_Half_Length().y))
		- glm::length(intersection),
		0.0f);
	intersection_length += 5.0f;

	intersection = glm::normalize(intersection) * intersection_length;

	dynamic_rb->Set_Current_Velocity(w + intersection);
}

void Collider::dynamic_circle_aabb_response(Circle& circ, AABB& aabb)
{
	Rigid_Body* a_rb = circ.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (a_rb == nullptr) return;

	Rigid_Body* b_rb = aabb.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (b_rb == nullptr) return;

	Transform* circ_trans = circ.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (circ_trans == nullptr) return;

	Transform* aabb_trans = aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (aabb_trans == nullptr) return;

	// Circle->AABB
	glm::vec2 distance = (aabb_trans->Get_Translation() + aabb.Get_Offset())
		- (circ_trans->Get_Translation() + circ.Get_Offset());
	glm::vec2 norm = glm::normalize(distance);

	glm::vec2 a_v = a_rb->Get_Current_Velocity() * a_rb->Get_Direction();
	glm::vec2 b_v = b_rb->Get_Current_Velocity() * b_rb->Get_Direction();

	float p = (2.0f * (glm::dot(a_v, norm) - glm::dot(b_v, norm))) / (a_rb->Get_Mass() + b_rb->Get_Mass());

	glm::vec2 a_w = a_v - p * a_rb->Get_Mass() * norm;
	glm::vec2 b_w = b_v + p * b_rb->Get_Mass() * norm;

	a_rb->Set_Current_Velocity(a_w - distance);
	b_rb->Set_Current_Velocity(b_w + distance);
}

// AABB-AABB
void Collider::static_aabb_aabb_response(AABB& a, AABB& b)
{
	Transform* a_trans = a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (a_trans == nullptr) return;

	Transform* b_trans = b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (b_trans == nullptr) return;

	// A->B
	glm::vec2 distance = (b_trans->Get_Translation() + b.Get_Offset())
		- (a_trans->Get_Translation() + a.Get_Offset());

	glm::vec2 a_offset = -glm::normalize(distance) * a.Get_Half_Length();
	glm::vec2 b_offset = glm::normalize(distance) * b.Get_Half_Length();

	a_trans->Set_Translation(a_trans->Get_Translation() - a.Get_Offset() + a_offset);
	b_trans->Set_Translation(b_trans->Get_Translation() - b.Get_Offset() + b_offset);
}

void Collider::dynamic_static_aabb_aabb_response(AABB& dynamic_aabb, AABB& static_aabb)
{
	Rigid_Body* dynamic_rb = dynamic_aabb.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (dynamic_rb == nullptr) return;

	Transform* static_trans = static_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (static_trans == nullptr) return;

	Transform* dynamic_trans = dynamic_aabb.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (dynamic_trans == nullptr) return;

	glm::vec2 n = (dynamic_trans->Get_Translation() + dynamic_aabb.Get_Offset())
		- (static_trans->Get_Translation() + static_aabb.Get_Offset());
	n = glm::normalize(n);

	float p = (2.0f * glm::dot(dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction(), n))
		/ (dynamic_rb->Get_Mass() + dynamic_rb->Get_Mass());

	glm::vec2 w = dynamic_rb->Get_Current_Velocity() * dynamic_rb->Get_Direction()
		- p * (dynamic_rb->Get_Mass() * n - dynamic_rb->Get_Mass() * n);

	// Dynamic->Static
	glm::vec2 distance = (static_trans->Get_Translation() + static_aabb.Get_Offset())
		- (dynamic_trans->Get_Translation() + dynamic_aabb.Get_Offset());

	float intersection_length = std::max(glm::length(glm::normalize(distance)
		* (static_aabb.Get_Half_Length()
			+ dynamic_aabb.Get_Half_Length() - glm::length(distance))),
		0.0f);
	intersection_length += 5.0f;

	distance = glm::normalize(distance) * -intersection_length;

	dynamic_rb->Set_Current_Velocity(w + distance);
}

void Collider::dynamic_aabb_aabb_response(AABB& a, AABB& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (a_rb == nullptr) return;

	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	if (b_rb == nullptr) return;

	Transform* circ_trans = a.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (circ_trans == nullptr) return;

	Transform* aabb_trans = b.Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);
	if (aabb_trans == nullptr) return;

	glm::vec2 distance = (aabb_trans->Get_Translation() + b.Get_Offset())
		- (circ_trans->Get_Translation() + a.Get_Offset());
	glm::vec2 norm = glm::normalize(distance);

	glm::vec2 a_v = a_rb->Get_Current_Velocity() * a_rb->Get_Direction();
	glm::vec2 b_v = b_rb->Get_Current_Velocity() * b_rb->Get_Direction();

	float p = (2.0f * (glm::dot(a_v, norm) - glm::dot(b_v, norm))) / (a_rb->Get_Mass() + b_rb->Get_Mass());

	glm::vec2 a_w = a_v - p * a_rb->Get_Mass() * norm;
	glm::vec2 b_w = b_v + p * b_rb->Get_Mass() * norm;

	a_rb->Set_Current_Velocity(a_w - distance);
	b_rb->Set_Current_Velocity(b_w + distance);
}

/************************************************* COLLIDER: Protected Functions *****************************************************/

// Collision Detection
bool Collider::circle_circle_check(Circle& a, Circle& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		return static_circle_circle_check(a, b);
	else if (a_rb == nullptr)
		return static_dynamic_circle_circle_check(b, a);
	else if (b_rb == nullptr)
		return static_dynamic_circle_circle_check(a, b);
	else 
		return (static_dynamic_circle_circle_check(b, a) || static_dynamic_circle_circle_check(a, b));
}

bool Collider::circle_aabb_check(Circle& a, AABB& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		return static_circle_aabb_check(a, b);
	else if (a_rb == nullptr)
		return static_circle_dynamic_aabb_check(a, b);
	else if (b_rb == nullptr)
		return dynamic_circle_static_aabb_check(a, b);
	else
		return (static_circle_dynamic_aabb_check(a, b) || dynamic_circle_static_aabb_check(a, b));
}

bool Collider::aabb_aabb_check(AABB& a, AABB& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		return static_aabb_aabb_check(a, b);
	else if (a_rb == nullptr)
		return static_dynamic_aabb_aabb_check(b, a);
	else if (b_rb == nullptr)
		return static_dynamic_aabb_aabb_check(a, b);
	else
		return (static_dynamic_aabb_aabb_check(b, a) || static_dynamic_aabb_aabb_check(a, b));
}

// Collision Resolution
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

void Collider::circle_aabb_response(Circle& a, AABB& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		static_circle_aabb_response(a, b);
	else if (a_rb == nullptr)
		static_circle_dynamic_aabb_response(a, b);
	else if (b_rb == nullptr)
		dynamic_circle_static_aabb_response(a, b);
	else
		dynamic_circle_aabb_response(a, b);
}

void Collider::aabb_aabb_response(AABB& a, AABB& b)
{
	Rigid_Body* a_rb = a.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);
	Rigid_Body* b_rb = b.Get_Parent()->Get_Component<Rigid_Body>(Component_Type::ct_Rigid_Body);

	if (a_rb == nullptr && b_rb == nullptr)
		static_aabb_aabb_response(a, b);
	else if (a_rb == nullptr)
		dynamic_static_aabb_aabb_response(b, a);
	else if (b_rb == nullptr)
		dynamic_static_aabb_aabb_response(a, b);
	else
		dynamic_aabb_aabb_response(a, b);
}

/**************************************************** CIRCLE: Private Functions ******************************************************/

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

/***************************************************** CIRCLE: Public Functions ******************************************************/

Component* Circle::Clone()
{
	Circle* circle = new Circle();
	circle->offset_ = offset_;

	circle->is_trigger_ = is_trigger_;
	circle->on_enter_ = on_enter_;
	circle->on_exit_ = on_exit_;
	circle->while_triggered_ = while_triggered_;

	circle->color_ = color_;

	circle->radius_ = radius_;

	circle->segments_ = segments_;

	return (Component*)circle;
}

void Circle::Start()
{
	shader_ = *Shader_Manager::Get_Instance()->Get_Resource("line");
	color_ = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
	initialize_circle_outline();

	Collider_Manager::Get_Instance()->Add_Collider(this);
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

void Circle::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values)
{
	writer.Key("circle");
	writer.StartObject();

	writer.Key("offset");
	writer.StartArray();
	writer.Double(offset_.x);
	writer.Double(offset_.y);
	writer.EndArray();

	writer.Key("is_trigger");
	writer.Bool(is_trigger_);
	// Something to track the trigger functions

	writer.Key("color");
	writer.StartArray();
	writer.Double(color_.r);
	writer.Double(color_.g);
	writer.Double(color_.b);
	writer.Double(color_.a);
	writer.EndArray();

	// I don't think the shader name is needed since it will be overwritten in the start

	writer.Key("radius");
	writer.Double(radius_);

	writer.Key("segments");
	writer.Int(segments_);

	writer.EndObject();
}

void Circle::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader)
{
	if (reader.HasMember("circle") == false) return;

	collider_type_ = Collider_Type::col_Circle;

	if (reader["circle"].GetObject().HasMember("offset") &&
		reader["circle"]["offset"].IsArray())
	{
		const rapidjson::Value& offset = reader["circle"]["offset"];

		offset_ = glm::vec2((float)offset[0].GetDouble(), 
			(float)offset[1].GetDouble());
	}

	if (reader["circle"].GetObject().HasMember("is_trigger") &&
		reader["circle"]["is_trigger"].IsBool())
		is_trigger_ = reader["circle"]["is_trigger"].GetBool();

	if (reader["circle"].GetObject().HasMember("color") &&
		reader["circle"]["color"].IsArray())
	{
		const rapidjson::Value& color = reader["circle"]["color"];

		color_ = glm::vec4((float)color[0].GetDouble(),
			(float)color[1].GetDouble(),
			(float)color[2].GetDouble(),
			(float)color[3].GetDouble());
	}

	if (reader["circle"].GetObject().HasMember("radius") &&
		reader["circle"]["radius"].IsDouble())
		radius_ = reader["circle"]["radius"].GetDouble();

	if (reader["circle"].GetObject().HasMember("segments") &&
		reader["circle"]["segments"].IsInt())
		segments_ = reader["circle"]["segments"].GetInt();
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
		return circle_aabb_check(*this, (AABB&)other);
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
		circle_aabb_response(*this, (AABB&)other);
		break;
	default:
		break;
	}
}

/******************************************************* AABB: Private Functions *****************************************************/

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

/******************************************************** AABB: Public Functions *****************************************************/

Component* AABB::Clone()
{
	AABB* aabb = new AABB();
	aabb->offset_ = offset_;

	aabb->is_trigger_ = is_trigger_;
	aabb->on_enter_ = on_enter_;
	aabb->on_exit_ = on_exit_;
	aabb->while_triggered_ = while_triggered_;

	aabb->color_ = color_;

	aabb->half_length_ = half_length_;

	return (Component*)aabb;
}

void AABB::Start()
{
	shader_ = *Shader_Manager::Get_Instance()->Get_Resource("line");
	color_ = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
	initialize_square_outline();

	Collider_Manager::Get_Instance()->Add_Collider(this);
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

void AABB::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values)
{
	writer.Key("aabb");
	writer.StartObject();

	writer.Key("offset");
	writer.StartArray();
	writer.Double(offset_.x);
	writer.Double(offset_.y);
	writer.EndArray();

	writer.Key("is_trigger");
	writer.Bool(is_trigger_);
	// Something to track the trigger functions

	writer.Key("color");
	writer.StartArray();
	writer.Double(color_.r);
	writer.Double(color_.g);
	writer.Double(color_.b);
	writer.Double(color_.a);
	writer.EndArray();

	// I don't think the shader name is needed since it will be overwritten in the start

	writer.Key("half_length");
	writer.StartArray();
	writer.Double(half_length_.x);
	writer.Double(half_length_.y);
	writer.EndArray();

	writer.EndObject();
}

void AABB::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& reader)
{
	if (reader.HasMember("aabb") == false) return;

	collider_type_ = Collider_Type::col_AABB;

	if (reader["aabb"].GetObject().HasMember("offset") &&
		reader["aabb"]["offset"].IsArray())
	{
		const rapidjson::Value& offset = reader["aabb"]["offset"];

		offset_ = glm::vec2((float)offset[0].GetDouble(),
			(float)offset[1].GetDouble());
	}

	if (reader["aabb"].GetObject().HasMember("is_trigger") &&
		reader["aabb"]["is_trigger"].IsBool())
		is_trigger_ = reader["aabb"]["is_trigger"].GetBool();

	if (reader["aabb"].GetObject().HasMember("color") &&
		reader["aabb"]["color"].IsArray())
	{
		const rapidjson::Value& color = reader["aabb"]["color"];

		color_ = glm::vec4((float)color[0].GetDouble(),
			(float)color[1].GetDouble(),
			(float)color[2].GetDouble(),
			(float)color[3].GetDouble());
	}

	if (reader["aabb"].GetObject().HasMember("half_length") &&
		reader["aabb"]["half_length"].IsArray())
	{
		const rapidjson::Value& half_length = reader["aabb"]["half_length"];

		half_length_ = glm::vec2((float)half_length[0].GetDouble(),
			(float)half_length[1].GetDouble());
	}
}

bool AABB::Collision_Detection(Collider& other)
{
	switch (other.Get_Collider_Type())
	{
	case Collider_Type::col_Undefined:
		return false;
		break;
	case Collider_Type::col_Circle:
		return circle_aabb_check((Circle&)other, *this);
		break;
	case Collider_Type::col_AABB:
		return aabb_aabb_check(*this, (AABB&)other);
		break;
	default:
		break;
	}

	return false;
}

void AABB::Collision_Response(Collider& other)
{
	switch (other.Get_Collider_Type())
	{
	case Collider_Type::col_Undefined:
		break;
	case Collider_Type::col_Circle:
		circle_aabb_response((Circle&)other, *this);
		break;
	case Collider_Type::col_AABB:
		aabb_aabb_response(*this, (AABB&)other);
		break;
	default:
		break;
	}
}