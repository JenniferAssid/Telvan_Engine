/*************************************************************************************************************************************
* Title: collider_manager.h
* Author: Jennifer Assid
* Date of Creation: 16 April 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef COLLIDER_MANAGER_CLASS_H
#define COLLIDER_MANAGER_CLASS_H

#include <vector>

class Collider;

class Collider_Manager
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
	std::vector<Collider*> active_colliders_;
	Collider_Manager() {}

	static Collider_Manager* instance_;
	
protected:
public:

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
	static Collider_Manager* Get_Instance();

	void Start();

	void Update(float dT);
	void Render();

	void Add_Collider(Collider* collider);
	void Remove_Collider(Collider* collider);

	void Clear();
	void Shutdown();
};

#endif // !COLLIDER_MANAGER_CLASS_H