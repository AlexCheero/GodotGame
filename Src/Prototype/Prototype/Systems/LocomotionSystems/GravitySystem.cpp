#include "GravitySystem.h"

void godot::GravitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, GravityComponent>();
	view.each([this, delta](VelocityComponent& velocityComp, GravityComponent& gravity)
	{
		float acc = velocityComp.velocity.y > 0 ? gravity.accUp : gravity.accDown;
		velocityComp.velocity.y -= acc * delta;
	});
}
