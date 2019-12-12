#include "CameraFollowSystem.h"

#include <Camera.hpp>
#include <Spatial.hpp>

#include "core/math/math_funcs.h"

void godot::CameraFollowSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<Camera*, Spatial*, CamRelativePositionComponent>().each([](Camera* pCam, Spatial* pTarget, CamRelativePositionComponent relPos)
	{
		pCam->get_transform().orthonormalize();
		Transform targetTransform = pTarget->get_global_transform();
		Vector3 targetPosition = targetTransform.origin;
		Basis targetBasis = targetTransform.get_basis();
		Vector3 camDirection = targetBasis.z.rotated(-targetBasis.x, Math::deg2rad(relPos.angle));
		Vector3 camPosition = targetPosition + camDirection * relPos.distance;
		pCam->look_at_from_position(camPosition, targetPosition, Vector3(0, 1, 0));
	});
}
