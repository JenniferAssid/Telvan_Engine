#pragma once

#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>

#include "components.h"

class Camera : Component
{
private:
	float zoom_;

public:
	Camera() : Component(Component_Type::ct_Camera),
		zoom_(1.0f)
	{}

	glm::mat4 Get_Matrix();
};

#endif // !CAMERA_CLASS_H
