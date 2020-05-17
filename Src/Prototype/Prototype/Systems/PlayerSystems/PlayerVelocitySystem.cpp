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
	view.less([](VelocityComponent& velocityComp, MoveDirInputComponent moveInput, SpeedComponent speedComp, Camera* pCam)
	{
		Vector3 flatVelocity = Vector3(0, 0, 0);
		Basis camBasis = pCam->get_transform().get_basis();
		flatVelocity += camBasis.x * moveInput.dir.x;
		flatVelocity -= camBasis.z * moveInput.dir.y;

		flatVelocity.normalize();
		flatVelocity *= speedComp.speed;

		velocityComp.velocity.x = flatVelocity.x;
		velocityComp.velocity.z = flatVelocity.z;
	});
}