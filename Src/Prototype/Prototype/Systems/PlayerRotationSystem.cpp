#include "PlayerRotationSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>

#include "../Components/Player.h"

void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	registry.view<RotationComponent, Player*, Camera*>().each([pInput](RotationComponent rotationComp, Player* pPlayer, Camera* pCam)
	{
		Basis camBasis = pCam->get_global_transform().get_basis();
		Vector3 dir{ 0, 0, 0 };
		if (pInput->is_action_pressed("ui_left"))
			dir += camBasis.x;
		if (pInput->is_action_pressed("ui_right"))
			dir -= camBasis.x;
		if (pInput->is_action_pressed("ui_up"))
			dir += camBasis.z;
		if (pInput->is_action_pressed("ui_down"))
			dir -= camBasis.z;
		dir.y = 0;

		if (dir.length_squared() == 0)
			return;
		
		Vector3 playerPosition = pPlayer->get_global_transform().get_origin();
		Vector3 lookTarget = playerPosition - dir;
		pPlayer->look_at(lookTarget, Vector3{ 0, 1, 0 });
	});
}
