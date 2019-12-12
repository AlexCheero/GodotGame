#include "PlayerVelocitySystem.h"

#include "../Components/Player.h"

void godot::PlayerVelocitySystem::Update(VelocityComponent& velocityComp, SpeedComponent speedComp, Camera* pCam, int directionMask)
{
	Basis camBasis = pCam->get_transform().get_basis();
	Vector3 flatVelocity = Vector3(0, 0, 0);
	if (directionMask & (1 << 0))
		flatVelocity -= camBasis.x;
	if (directionMask & (1 << 1))
		flatVelocity += camBasis.x;
	if (directionMask & (1 << 2))
		flatVelocity -= camBasis.z;
	if (directionMask & (1 << 3))
		flatVelocity += camBasis.z;

	flatVelocity.normalize();
	flatVelocity *= speedComp.speed;

	velocityComp.velocity.x = flatVelocity.x;
	velocityComp.velocity.z = flatVelocity.z;
}

void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	int mask = 0;
	if (pInput->is_action_pressed("move_left"))
		mask |= 1;
	if (pInput->is_action_pressed("move_right"))
		mask |= 1 << 1;
	if (pInput->is_action_pressed("move_up"))
		mask |= 1 << 2;
	if (pInput->is_action_pressed("move_down"))
		mask |= 1 << 3;

	registry.view<VelocityComponent, SpeedComponent, Player*, Camera*>().each([&](VelocityComponent& velocity, SpeedComponent speedComp, Player* pPlayer, Camera* pCam)
	{
		Update(velocity, speedComp, pCam, mask);
	});
}