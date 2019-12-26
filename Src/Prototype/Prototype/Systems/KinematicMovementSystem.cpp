#include "KinematicMovementSystem.h"

void godot::KinematicMovementSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<VelocityComponent, KinematicBody*>().each([](VelocityComponent& velocityComp, KinematicBody* pNode)
	{
		//TODO: remove logs when done with NavAgentSystem
		Godot::print("vel: " + String::num_real(velocityComp.velocity.x) + ", " + String::num_real(velocityComp.velocity.y) + ", " + String::num_real(velocityComp.velocity.z));
		Vector3 move = pNode->move_and_slide(velocityComp.velocity, Vector3(0, 1, 0));
		Godot::print("move: " + String::num_real(move.x) + ", " + String::num_real(move.y) + ", " + String::num_real(move.z));
	});
}
