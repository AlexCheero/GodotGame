#include "LookAtSystem.h"

#include <Spatial.hpp>

void godot::LookAtSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<RotationDirectionComponent, Spatial*>();
	view.each([](RotationDirectionComponent rotationDir, Spatial* pSpatial)
	{
		Vector3 lookTarget = pSpatial->get_global_transform().get_origin() + rotationDir.direction;
		pSpatial->look_at(lookTarget, Vector3{ 0, 1, 0 });
	});
}