#include "GravitySystem.h"

#include <KinematicBody.hpp>

void godot::GravitySystem::Update(VelocityComponent& velocity, GravityComponent& gravity, float delta, bool isOnFloor)
{
	if (isOnFloor)
		velocity.y = 0;
	else
	{
		float acc = velocity.y > 0 ? gravity.accUp : gravity.accDown;
		velocity.y -= acc * delta;
	}
}

void godot::GravitySystem::operator()(float delta, entt::registry& registry)
{
	registry.view<VelocityComponent, GravityComponent, KinematicBody*>().each([&](VelocityComponent& velocity, GravityComponent& gravity, KinematicBody* pBody)
	{
		Update(velocity, gravity, delta, pBody->is_on_floor());
	});
}
