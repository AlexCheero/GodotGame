#include "PlayerRotationRSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../Components/InputComponents.h"
#include "../Components/SimpleComponents.h"

#include "../Utils/Utils.h"

namespace //private
{
	godot::Vector3 GetTargetDirection(godot::Vector2 inputDir, godot::Basis camBasis)
	{
		godot::Vector3 dir{ 0, 0, 0 };
		const godot::Plane xzPlane(utils::globalY, 0);
		godot::Vector3 relativeLeft = xzPlane.project(camBasis.x).normalized();
		godot::Vector3 relativeFwd = xzPlane.project(camBasis.z).normalized();
		dir += relativeLeft * inputDir.x - relativeFwd * inputDir.y;
		dir.y = 0;

		return dir.normalized();
	}

	//TODO: make smooth rotation
	void OnRotationChanged(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<PlayerTag>(entity))
			return;

		ASSERT(registry.has<RotationDirectionComponent>(entity), "entity has no RotationDirectionComponent");
		ASSERT(registry.has<RotationInputComponent>(entity), "entity has no RotationInputComponent");
		ASSERT(registry.has<godot::Camera*>(entity), "entity has no Camera*");

		RotationDirectionComponent& rotDir = registry.get<RotationDirectionComponent>(entity);
		RotationInputComponent input = registry.get<RotationInputComponent>(entity);
		godot::Camera* pCam = registry.get<godot::Camera*>(entity);

		if (input.dir.length_squared() == 0)
			return;

		godot::Basis camBasis = pCam->get_global_transform().get_basis();
		rotDir.direction = GetTargetDirection(input.dir, camBasis);
	}
}

void godot::PlayerRotationRSystem::Init(entt::registry& registry)
{
	registry.on_construct<RotationChangedTag>().connect<&OnRotationChanged>();
}