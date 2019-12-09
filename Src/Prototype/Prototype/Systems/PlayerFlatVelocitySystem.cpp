#include "PlayerFlatVelocitySystem.h"

void godot::PlayerFlatVelocitySystem::Update(FlatVelocityComponent& movement, SpeedComponent& speed, float delta, int directionMask)
{
	movement = Vector2(0, 0);
	if (directionMask & (1 << 0))
		movement.x -= 1;
	if (directionMask & (1 << 1))
		movement.x += 1;
	if (directionMask & (1 << 2))
		movement.y -= 1;
	if (directionMask & (1 << 3))
		movement.y += 1;

	movement.normalize();
	movement *= speed * delta;
}

void godot::PlayerFlatVelocitySystem::operator()(float delta, entt::registry& registry)
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

	registry.view<FlatVelocityComponent, SpeedComponent>().each([&](FlatVelocityComponent& movement, SpeedComponent& speed)
	{
		Update(movement, speed, delta, mask);
	});
}