#include "KinematicMovementSystem.h"

void godot::KinematicMovementSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<FlatVelocityComponent, KinematicBody*>().each([](FlatVelocityComponent& flatVel, KinematicBody* pNode)
	{
		Vector3 movement(flatVel.x, 0, flatVel.y);
		pNode->move_and_slide(movement, Vector3(0, 1, 0));
	});
}
