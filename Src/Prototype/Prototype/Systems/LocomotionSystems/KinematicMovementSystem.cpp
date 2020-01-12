#include "KinematicMovementSystem.h"

void godot::KinematicMovementSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<VelocityComponent, KinematicBody*>().each([](VelocityComponent& velocityComp, KinematicBody* pKBody)
	{
		//TODO: the player's and enemy's noses are looking in oposite directions
		pKBody->move_and_slide(velocityComp.velocity, Vector3(0, 1, 0));
	});
}
