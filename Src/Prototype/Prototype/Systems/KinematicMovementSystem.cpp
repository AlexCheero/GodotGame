#include "KinematicMovementSystem.h"

void godot::KinematicMovementSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<VelocityComponent, KinematicBody*>().each([](VelocityComponent& vel, KinematicBody* pNode)
	{
		pNode->move_and_slide(vel, Vector3(0, 1, 0));
	});
}
