#include "CameraFollowSystem.h"

#include <Camera.hpp>
#include <Spatial.hpp>

void godot::CameraFollowSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<Camera*, Spatial*, CamRelativePositionComponent>().each([](Camera* pCam, Spatial* pTarget, CamRelativePositionComponent relPos)
	{
		//
	});
}
