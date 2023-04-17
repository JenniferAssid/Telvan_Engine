/*************************************************************************************************************************************
* Title: graphics.h
* Author: Jennifer Assid
* Date of Creation: 23 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef GRAPHICS_CLASS_H
#define GRAPHICS_CLASS_H

#include <glad/glad.h>
#include <glad/glad.h>

#include "system.h"
#include "error_logging.h"

class Graphics : public System
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
	static Graphics* instance_;

protected:
public:

	/************************************************************ FUNCTIONS **************************************************************/
private:
	Graphics() : System("Graphics", "")
	{}

protected:
public:
	inline static Graphics* Get_Instance()
	{
		if (instance_ == nullptr)
		{
			Error_Logging::Get_Instance()->Record_Message(
				"Instance Created",
				Error_Logging::Message_Level::ot_Information,
				"Graphics",
				"Get_Instance"
			);
			instance_ = new Graphics();
		}

		return instance_;
	}

	void Initialize() override;
	void Shutdown() override;
};

#endif // !GRAPHICS_CLASS_H
