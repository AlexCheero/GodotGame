#include "CameraFollowSystem.h"

#include <Camera.hpp>
#include <Spatial.hpp>

#include "core/math/math_funcs.h"

godot::Vector3 godot::CameraFollowSystem::NewCamPosition(Vector3 targetPosition, CamPositionComponent relPos)
{
	const Vector3 globalX = Vector3{ 1, 0, 0 };
	const Vector3 globalY = Vector3{ 0, 1, 0 };
	const Vector3 globalZ = Vector3{ 0, 0, 1 };

	Vector3 camDirection = globalZ.rotated(globalY, Math::deg2rad(relPos.yAngle));
	camDirection = camDirection.rotated(globalX, Math::deg2rad(relPos.xAngle));

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
