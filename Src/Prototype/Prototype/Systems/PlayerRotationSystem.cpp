#include "PlayerRotationSystem.h"

#include <Transform.hpp>

#include "../Components/Player.h"

void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	//int mask = 0;
	//if (pInput->is_action_pressed("ui_left"))
	//	mask |= 1;
	//if (pInput->is_action_pressed("ui_right"))
	//	mask |= 1 << 1;
	//if (pInput->is_action_pressed("ui_up"))
	//	mask |= 1 << 2;
	//if (pInput->is_action_pressed("ui_down"))
	//	mask |= 1 << 3;

	if (pInput->is_action_just_pressed("ui_left"))		
		//TODO: check captures of all systems so far
		registry.view<RotationComponent, Player*>().each([&](RotationComponent rotationComp, Player* pPlayer)
		{
			Spatial* camSpatial = Object::cast_to<Spatial>(pPlayer->get_node("Camera"));
			Transform camTransform = camSpatial->get_global_transform();
			Vector3 playerPosition = pPlayer->get_global_transform().get_origin();
			Vector3 lookTarget = playerPosition + camTransform.get_basis().x * 10;
			pPlayer->set_transform(pPlayer->get_transform().looking_at(Vector3{}, Vector3{ 0, 1, 0 }));
		});
}
