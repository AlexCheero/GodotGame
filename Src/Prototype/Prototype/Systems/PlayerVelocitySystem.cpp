#include "PlayerVelocitySystem.h"

void godot::PlayerVelocitySystem::Update(VelocityComponent& velocityComp, SpeedComponent speedComp, int directionMask)
{
	Vector2 flatVelocity = Vector2(0, 0);
	if (directionMask & (1 << 0))
		flatVelocity.x -= 1;
	if (directionMask & (1 << 1))
		flatVelocity.x += 1;
	if (directionMask & (1 << 2))
		flatVelocity.y -= 1;
	if (directionMask & (1 << 3))
		flatVelocity.y += 1;

	flatVelocity.normalize();
	flatVelocity *= speedComp.speed;

	velocityComp.velocity.x = flatVelocity.x;
	velocityComp.velocity.z = flatVelocity.y;
}

void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	int mask = 0;
	if (pInput->is_action_pressed("ui_left"))
		mask |= 1;
	if (pInput->is_action_pressed("ui_right"))
		mask |= 1 << 1;
	if (pInput->is_action_pressed("ui_up"))
		mask |= 1 << 2;
	if (pInput->is_action_pressed("ui_down"))
		mask |= 1 << 3;

	registry.view<VelocityComponent, SpeedComponent>().each([&](VelocityComponent& velocity, SpeedComponent speedComp)
	{
		Update(velocity, speedComp, mask);
	});
}