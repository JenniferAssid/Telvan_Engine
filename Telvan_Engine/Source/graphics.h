#pragma once

#ifndef GRAPHICS_CLASS_H
#define GRAPHICS_CLASS_H

#include <glad/glad.h>
#include <glad/glad.h>

#include "system.h"

class Graphics : public System
{
private:
	Graphics() : System("Graphics", "")
	{}
	static Graphics* instance_;

public:
	inline static Graphics* Get_Instance()
	{
		if (instance_ == nullptr)
			instance_ = new Graphics();

		return instance_;
	}

	void Initialize() override;
	void Shutdown() override;
};

#endif // !GRAPHICS_CLASS_H
