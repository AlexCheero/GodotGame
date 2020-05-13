#include "PlayerVelocityRSystem.h"

#include <Input.hpp>
#include <Camera.hpp>

#include "../Components/InputComponents.h"
#include "../Components/SimpleComponents.h"

#include "../Utils/Utils.h"

//TODO: speed should be different, depending on player's orientation. probably should merge
namespace //private
{
	void OnMoveDirChanged(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<PlayerTag>(entity))
			return;

		ASSERT(registry.has<VelocityComponent>(entity), "entity has no VelocityComponent");
		ASSERT(registry.has<MoveDirInputComponent>(entity), "entity has no MoveDirInputComponent");
		ASSERT(registry.has<SpeedComponent>(entity), "entity has no SpeedComponent");
		ASSERT(registry.has<godot::Camera*>(entity), "entity has no Camera*");

		VelocityComponent& velocityComp = registry.get<VelocityComponent>(entity);
		MoveDirInputComponent moveInput = registry.get<MoveDirInputComponent>(entity);
		SpeedComponent speedComp = registry.get<SpeedComponent>(entity);
		godot::Camera* pCam = registry.get<godot::Camera*>(entity);
		
		godot::Vector3 flatVelocity = godot::Vector3(0, 0, 0);
		godot::Basis camBasis = pCam->get_transform().get_basis();
		flatVelocity += camBasis.x * moveInput.dir.x;
		flatVelocity -= camBasis.z * moveInput.dir.y;

		flatVelocity.normalize();
		flatVelocity *= speedComp.speed;

		velocityComp.velocity.x = flatVelocity.x;
		velocityComp.velocity.z = flatVelocity.z;
	}
}

void godot::PlayerVelocityRSystem::Init(entt::registry& registry)
{
	registry.on_construct<MoveDirChangedTag>().connect<&OnMoveDirChanged>();
}