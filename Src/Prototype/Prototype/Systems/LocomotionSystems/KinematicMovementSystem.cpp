#include "KinematicMovementSystem.h"

void godot::KinematicMovementSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<VelocityComponent, KinematicBody*>().each([](VelocityComponent& velocityComp, KinematicBody* pNode)
	{
		Vector3 move = pNode->move_and_slide(velocityComp.velocity, Vector3(0, 1, 0));
	});
}
