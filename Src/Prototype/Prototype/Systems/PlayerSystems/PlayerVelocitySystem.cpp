#include "PlayerVelocitySystem.h"

#include <Input.hpp>
#include <Camera.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

#include "../../Utils/Utils.h"

//TODO: speed should be different, depending on player's orientation. probably should merge
void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, VelocityComponent, MoveDirInputComponent, SpeedComponent, Camera*>();
	view.less([this](VelocityComponent& velocityComp, MoveDirInputComponent moveInput, SpeedComponent speedComp, Camera* pCam)
	{
		Basis camBasis = pCam->get_global_transform().get_basis();
		Vector3 flatVelocity = utils::GetRelativeFlatDirection(moveInput.dir, camBasis.x, camBasis.z) * speedComp.speed;
		velocityComp.velocity.x = flatVelocity.x;
		velocityComp.velocity.z = flatVelocity.z;
	});
}