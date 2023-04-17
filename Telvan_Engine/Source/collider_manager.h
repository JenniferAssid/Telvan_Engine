#pragma once

#ifndef COLLIDER_MANAGER_CLASS_H
#define COLLIDER_MANAGER_CLASS_H

#include <vector>

class Collider;

class Collider_Manager
{
private:
	std::vector<Collider*> active_colliders_;
	Collider_Manager() {}

	static Collider_Manager* instance_;

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