/*************************************************************************************************************************************
* Title: sprite_renderer_manager.cpp
* Author: Jennifer Assid
* Date of Creation: 1 July 2024
*
**************************************************************************************************************************************/
#include "sprite_renderer_manager.h"
#include "sprite_renderer.h"
#include "transform.h"

#include "error_logging.h"

Sprite_Renderer_Manager* Sprite_Renderer_Manager::instance_;

Sprite_Renderer_Manager* Sprite_Renderer_Manager::Get_Instance()
{
	if (instance_ == nullptr)
	{
		Error_Logging::Get_Instance()->Record_Message(
			"Instance Created",
			Error_Logging::Message_Level::ot_Information,
			"Sprite_Renderer_Manager",
			"Get_Instance"
		);
		instance_ = new Sprite_Renderer_Manager();
	}
	return instance_;
}

void Sprite_Renderer_Manager::Start()
{
}

void Sprite_Renderer_Manager::Update(float dT)
{
	
}

void Sprite_Renderer_Manager::Render()
{
	for (Sprite_Renderer* sprite_renderer : active_sprite_renderers_)
		sprite_renderer->Render();
}

void Sprite_Renderer_Manager::Add_Sprite_Renderer(Sprite_Renderer* sprite_renderer)
{
	if (sprite_renderer->Get_Parent() == nullptr) return;

	Transform* trans = sprite_renderer->Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform);

	if (trans == nullptr) return;

	for (std::vector<Sprite_Renderer*>::iterator it = active_sprite_renderers_.begin();
		it != active_sprite_renderers_.end();
		it++)
	{
		if ((*it)->Get_Parent()->Get_Component<Transform>(Component_Type::ct_Transform)->Get_Z_Sorting_Value() >
			trans->Get_Z_Sorting_Value())
		{
			active_sprite_renderers_.insert(it, sprite_renderer);
			return;
		}
	}

	active_sprite_renderers_.push_back(sprite_renderer);
}

void Sprite_Renderer_Manager::Remove_Sprite_Renderer(Sprite_Renderer* sprite_renderer)
{
	std::vector<Sprite_Renderer*>::iterator itr = std::find(active_sprite_renderers_.begin(),
		active_sprite_renderers_.end(),
		sprite_renderer);

	if (itr == active_sprite_renderers_.end()) return;

	active_sprite_renderers_.erase(itr);
}

void Sprite_Renderer_Manager::Clear()
{
	active_sprite_renderers_.clear();
}

void Sprite_Renderer_Manager::Shutdown()
{
	Clear();
}