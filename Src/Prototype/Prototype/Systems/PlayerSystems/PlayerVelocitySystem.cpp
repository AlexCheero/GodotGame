#include "PlayerVelocitySystem.h"

#include <Input.hpp>
#include <Camera.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

#include "../../Utils/Utils.h"

godot::Vector3 godot::PlayerVelocitySystem::GetTargetDirection(Vector2 inputDir, Basis camBasis)
{
	Vector3 dir{ 0, 0, 0 };
	const Plane xzPlane(utils::globalY, 0);
	Vector3 relativeLeft = xzPlane.project(camBasis.x).normalized();
	Vector3 relativeFwd = xzPlane.project(camBasis.z).normalized();
	dir += relativeLeft * inputDir.x - relativeFwd * inputDir.y;
	dir.y = 0;

	return dir.normalized();
}

//TODO: speed should be different, depending on player's orientation. probably should merge
void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, VelocityComponent, MoveDirInputComponent, SpeedComponent, Camera*>();
	view.less([this](VelocityComponent& velocityComp, MoveDirInputComponent moveInput, SpeedComponent speedComp, Camera* pCam)
	{
		Basis camBasis = pCam->get_global_transform().get_basis();
		Vector3 flatVelocity = GetTargetDirection(moveInput.dir, camBasis) * speedComp.speed;
		velocityComp.velocity.x = flatVelocity.x;
		velocityComp.velocity.z = flatVelocity.z;
	});
}