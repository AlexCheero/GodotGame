#include "SimpleFollowSystem.h"

#include <Camera.hpp>
#include <Spatial.hpp>

#include "core/math/math_funcs.h"

#include "../Utils.h"

godot::Vector3 godot::SimpleFollowSystem::NewCamPosition(Vector3 targetPosition, SimpleFollowComponent comp)
{
	Vector3 camDirection = utils::globalZ.rotated(utils::globalY, Math::deg2rad(comp.yAngle));
	camDirection = camDirection.rotated(utils::globalX, Math::deg2rad(comp.xAngle));

	return targetPosition + camDirection.normalized() * comp.distance;
}

void godot::SimpleFollowSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<Camera*, SimpleFollowComponent>().each([&registry, this](entt::entity entity, Camera* pCam, SimpleFollowComponent comp)
	{
		if (!registry.valid(comp.targetEntity))
		{
			registry.remove<SimpleFollowComponent>(entity);
			return;
		}

		Spatial* pTarget = registry.get<Spatial*>(comp.targetEntity);
		Vector3 targetPosition = pTarget->get_global_transform().origin;
		pCam->look_at_from_position(NewCamPosition(targetPosition, comp), targetPosition, Vector3(0, 1, 0));
	});
}
