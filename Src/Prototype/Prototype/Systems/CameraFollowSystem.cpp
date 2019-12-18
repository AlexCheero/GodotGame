#include "CameraFollowSystem.h"

#include <Camera.hpp>
#include <Spatial.hpp>

#include "core/math/math_funcs.h"

#include "../Utils.h"

godot::Vector3 godot::CameraFollowSystem::NewCamPosition(Vector3 targetPosition, CamPositionComponent relPos)
{
	Vector3 camDirection = utils::globalZ.rotated(utils::globalY, Math::deg2rad(relPos.yAngle));
	camDirection = camDirection.rotated(utils::globalX, Math::deg2rad(relPos.xAngle));

	return targetPosition + camDirection.normalized() * relPos.distance;
}

void godot::CameraFollowSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<Camera*, Spatial*, CamPositionComponent>().each([this](Camera* pCam, Spatial* pTarget, CamPositionComponent relPos)
	{
		Vector3 targetPosition = pTarget->get_global_transform().origin;
		pCam->look_at_from_position(NewCamPosition(targetPosition, relPos), targetPosition, Vector3(0, 1, 0));
	});
}
