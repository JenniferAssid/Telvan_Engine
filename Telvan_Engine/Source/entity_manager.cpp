#include "entity_manager.h"
#include "entity.h"
#include "error_logging.h"

Entity_Manager* Entity_Manager::instance_;

Entity_Manager* Entity_Manager::Get_Instance()
{
	if (instance_ == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message(
			"Instance Created",
			Error_Logging::Message_Level::ot_Information,
			"Entity_Manager",
			"Get_Instance"
		);
		instance_ = new Entity_Manager();
	}
	return instance_;
}

void Entity_Manager::Start()
{
	for (Entity* entity : active_entity_)
		entity->Start();
}

void Entity_Manager::Update(float dT)
{
	for (unsigned int i = 0; i < active_entity_.size(); i++)
	{
		if (active_entity_[i]->Is_Destroyed())
		{
			active_entity_.erase(active_entity_.begin() + i);
			i--;
			continue;
		}

		active_entity_[i]->Pre_Update(dT);
	}

	Collider_Manager::Get_Instance()->Update(dT);
		
	for (Entity* entity : active_entity_)
		entity->Update(dT);
	for (Entity* entity : active_entity_)
		entity->Post_Update(dT);
	
}

void Entity_Manager::Render()
{
	for (Entity* entity : active_entity_)
		entity->Render();
}

void Entity_Manager::Add_Entity(Entity* entity)
{
	entity->Start();
	active_entity_.push_back(entity);
}

void Entity_Manager::Clear()
{
	active_entity_.clear();
}

void Entity_Manager::Shutdown()
{
	Clear();
}
