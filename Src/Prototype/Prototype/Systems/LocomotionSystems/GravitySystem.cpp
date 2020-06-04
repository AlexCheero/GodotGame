#include "GravitySystem.h"

void godot::GravitySystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, GravityComponent>();
	view.each([delta](VelocityComponent& velocityComp, GravityComponent& gravity)
	{
		float acc = velocityComp.velocity.y > 0 ? gravity.accelerationUp : gravity.accelerationDown;
		velocityComp.velocity.y -= acc * delta;
	});
}
