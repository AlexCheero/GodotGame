#include "CameraFollowSystem.h"

#include <Camera.hpp>
#include <Spatial.hpp>

#include "core/math/math_funcs.h"

void godot::CameraFollowSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<Camera*, Spatial*, CamPositionComponent>().each([](Camera* pCam, Spatial* pTarget, CamPositionComponent relPos)
	{
		Transform targetTransform = pTarget->get_global_transform();
		Vector3 targetPosition = targetTransform.origin;
		
		//global camera direction
		const Vector3 globalX = Vector3{ 1, 0, 0 };
		const Vector3 globalY = Vector3{ 0, 1, 0 };
		const Vector3 globalZ = Vector3{ 0, 0, 1 };

		Vector3 camDirection = globalZ.rotated(-globalY, Math::deg2rad(relPos.yAngle));
		camDirection = camDirection.rotated(-globalX, Math::deg2rad(relPos.xAngle));

		Vector3 camPosition = targetPosition + camDirection * relPos.distance;
		pCam->look_at_from_position(camPosition, targetPosition, Vector3(0, 1, 0));
	});
}
