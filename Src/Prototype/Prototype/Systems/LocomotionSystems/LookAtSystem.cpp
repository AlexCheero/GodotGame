#include "LookAtSystem.h"

#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"

void godot::LookAtSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<RotationDirectionComponent, Spatial*>(ExcludeDead);
	view.each([](RotationDirectionComponent rotationDir, Spatial* pTarget)
	{
		rotationDir.direction.y = 0;
		if (rotationDir.direction.length_squared() == 0)
			return;

		Vector3 lookTarget = pTarget->get_global_transform().get_origin() - rotationDir.direction;
		pTarget->look_at(lookTarget, Vector3{ 0, 1, 0 });
	});
}