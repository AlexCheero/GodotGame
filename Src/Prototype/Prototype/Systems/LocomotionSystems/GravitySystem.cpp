#include "GravitySystem.h"

#include <KinematicBody.hpp>

void godot::GravitySystem::Update(VelocityComponent& velocityComp, GravityComponent& gravity, float delta, bool isOnFloor)
{
	if (isOnFloor)
		velocityComp.velocity.y = 0;
	else
	{
		float acc = velocityComp.velocity.y > 0 ? gravity.accUp : gravity.accDown;
		velocityComp.velocity.y -= acc * delta;
	}
}

void godot::GravitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, GravityComponent, KinematicBody*>();
	view.each([this, delta](VelocityComponent& velocityComp, GravityComponent& gravity, KinematicBody* pBody)
	{
		Update(velocityComp, gravity, delta, pBody->is_on_floor());
	});
}
