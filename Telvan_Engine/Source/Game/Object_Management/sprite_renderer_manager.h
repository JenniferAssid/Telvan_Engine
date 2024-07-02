/*************************************************************************************************************************************
* Title: sprite_renderer_manager.h
* Author: Jennifer Assid
* Date of Creation: 1 July 2024
*
**************************************************************************************************************************************/
#pragma once

#ifndef SPRITE_RENDERER_MANAGER_CLASS_H
#define SPRITE_RENDERER_MANAGER_CLASS_H

#include <vector>

class Sprite_Renderer;

class Sprite_Renderer_Manager
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
	std::vector<Sprite_Renderer*> active_sprite_renderers_;
	Sprite_Renderer_Manager() {}

	static Sprite_Renderer_Manager* instance_;
	
protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
	static Sprite_Renderer_Manager* Get_Instance();

	void Start();

	void Update(float dT);
	void Render();

	void Add_Sprite_Renderer(Sprite_Renderer* collider);
	void Remove_Sprite_Renderer(Sprite_Renderer* collider);

	void Clear();
	void Shutdown();
};

#endif // !COLLIDER_MANAGER_CLASS_H