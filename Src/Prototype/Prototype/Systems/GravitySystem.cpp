#include "GravitySystem.h"

void godot::GravitySystem::operator()(float delta, entt::registry& registry)
{
	registry.view<FlatVelocityComponent, SpeedComponent>().each([&](FlatVelocityComponent& movement, SpeedComponent& speed)
	{
		//
	});
}
