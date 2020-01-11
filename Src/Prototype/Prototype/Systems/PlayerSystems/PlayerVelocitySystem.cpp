#include "PlayerVelocitySystem.h"

#include <Input.hpp>

#include "../../Components/InputComponents.h"

void godot::PlayerVelocitySystem::Update(VelocityComponent& velocityComp, SpeedComponent speedComp, Basis camBasis, Vector2 input)
{
	Vector3 flatVelocity = Vector3(0, 0, 0);
	flatVelocity -= camBasis.x * input.x;
	flatVelocity -= camBasis.z * input.y;

	flatVelocity.normalize();
	flatVelocity *= speedComp.speed;

	velocityComp.velocity.x = flatVelocity.x;
	velocityComp.velocity.z = flatVelocity.z;
}

void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<PlayerTag>, VelocityComponent, InputComponent, SpeedComponent, Camera*>();
	view.less([&registry](entt::entity entity, VelocityComponent& velocity, InputComponent input, SpeedComponent speedComp, Camera* pCam)
	{
		Update(velocity, speedComp, pCam->get_transform().get_basis(), input.moveDir);
	});
}