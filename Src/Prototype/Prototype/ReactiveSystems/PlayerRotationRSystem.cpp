#include "PlayerRotationRSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../Components/InputComponents.h"
#include "../Components/SimpleComponents.h"

#include "../Utils/Utils.h"

godot::Vector3 godot::PlayerRotationSystem::GetTargetDirection(Vector2 inputDir, Basis camBasis)
{
	godot::Vector3 dir{ 0, 0, 0 };
	const godot::Plane xzPlane(utils::globalY, 0);
	godot::Vector3 relativeLeft = xzPlane.project(camBasis.x).normalized();
	godot::Vector3 relativeFwd = xzPlane.project(camBasis.z).normalized();
	dir += relativeLeft * inputDir.x - relativeFwd * inputDir.y;
	dir.y = 0;

	return dir.normalized();
}

void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, RotationDirectionComponent, RotationInputComponent, Camera*>();
	view.less([this](RotationDirectionComponent& rotDir, RotationInputComponent input, Camera* pCam)
	{
		if (input.dir.length_squared() == 0)
			return;

		godot::Basis camBasis = pCam->get_global_transform().get_basis();
		rotDir.direction = GetTargetDirection(input.dir, camBasis);
	});
}