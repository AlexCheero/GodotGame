#include "PlayerRotationSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../Components/Player.h"

inline godot::Vector2 godot::PlayerRotationSystem::GetInputDir(int directionMask)
{
	Vector2 inputDir{ 0, 0 };
	if (directionMask & (1 << 0))
		inputDir.x = 1;
	if (directionMask & (1 << 1))
		inputDir.x = -1;
	if (directionMask & (1 << 2))
		inputDir.y = 1;
	if (directionMask & (1 << 3))
		inputDir.y = -1;
	inputDir.normalize();

	return inputDir;
}

inline godot::Vector3 godot::PlayerRotationSystem::GetTargetDirection(Vector2 inputDir, Basis camBasis)
{
	Vector3 dir{ 0, 0, 0 };
	dir += camBasis.x * inputDir.x + camBasis.z * inputDir.y;
	dir.y = 0;

	return dir;
}

void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	int directionMask = 0;
	if (pInput->is_action_pressed("ui_left"))
		directionMask |= 1;
	if (pInput->is_action_pressed("ui_right"))
		directionMask |= 1 << 1;
	if (pInput->is_action_pressed("ui_up"))
		directionMask |= 1 << 2;
	if (pInput->is_action_pressed("ui_down"))
		directionMask |= 1 << 3;

	registry.view<RotationTag, Player*, Camera*>().each([this, directionMask](RotationTag rotationComp, Player* pPlayer, Camera* pCam)
	{
		Basis camBasis = pCam->get_global_transform().get_basis();
		Vector3 dir = GetTargetDirection(GetInputDir(directionMask), camBasis);

		if (dir.length_squared() == 0)
			return;
		
		Vector3 lookTarget = pPlayer->get_global_transform().get_origin() - dir;
		pPlayer->look_at(lookTarget, Vector3{ 0, 1, 0 });
	});
}
