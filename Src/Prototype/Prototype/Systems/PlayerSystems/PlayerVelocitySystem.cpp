#include "PlayerVelocitySystem.h"

#include <Input.hpp>

#include "../../Components/InputComponents.h"

//TODO: speed should be different, depending on player's orientation. probably should merge
//		PlayerVelocitySystem, LocomotionAnimSystem, GravitySystem, JumpSystem, KinematicMovementSystem and PlayerRotationSystem
void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, VelocityComponent, InputComponent, SpeedComponent, Camera*>();
	view.less([&registry](VelocityComponent& velocityComp, InputComponent input, SpeedComponent speedComp, Camera* pCam)
	{
		Vector3 flatVelocity = Vector3(0, 0, 0);
		Basis camBasis = pCam->get_transform().get_basis();
		flatVelocity += camBasis.x * input.moveDir.x;
		flatVelocity -= camBasis.z * input.moveDir.y;

		flatVelocity.normalize();
		flatVelocity *= speedComp.speed;

		velocityComp.velocity.x = flatVelocity.x;
		velocityComp.velocity.z = flatVelocity.z;
	});
}