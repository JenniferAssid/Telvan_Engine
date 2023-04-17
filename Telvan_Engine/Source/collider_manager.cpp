#include "collider_manager.h"
#include "collider.h"
#include "error_logging.h"

Collider_Manager* Collider_Manager::instance_;

Collider_Manager* Collider_Manager::Get_Instance()
{
	if (instance_ == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message(
			"Instance Created",
			Error_Logging::Message_Level::ot_Information,
			"Collider_Manager",
			"Get_Instance"
		);
		instance_ = new Collider_Manager();
	}
	return instance_;
}

void Collider_Manager::Start()
{
	for (Collider* collider : active_colliders_)
		collider->Start();
}

void Collider_Manager::Update(float dT)
{
	for (unsigned int i = 0; i < active_colliders_.size(); i++)
	{
		for (unsigned int j = i + 1; j < active_colliders_.size(); j++)
		{
			if (active_colliders_[i]->Collision_Detection(*active_colliders_[j]))
			{
				active_colliders_[i]->Set_Color(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
				active_colliders_[j]->Set_Color(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
			}
			else
			{
				active_colliders_[i]->Set_Color(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
				active_colliders_[j]->Set_Color(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
			}
		}
	}
}

void Collider_Manager::Render()
{
	for (Collider* collider : active_colliders_)
		collider->Render();
}

void Collider_Manager::Add_Collider(Collider* collider)
{
	if (collider->Get_Parent() == nullptr) return;

	collider->Start();
	active_colliders_.push_back(collider);
}

void Collider_Manager::Remove_Collider(Collider* collider)
{
	std::vector<Collider*>::iterator itr = std::find(active_colliders_.begin(),
		active_colliders_.end(),
		collider);

	if (itr == active_colliders_.end()) return;

	active_colliders_.erase(itr);
}

void Collider_Manager::Clear()
{
	active_colliders_.clear();
}

void Collider_Manager::Shutdown()
{
	Clear();
}