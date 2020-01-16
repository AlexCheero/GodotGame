#include "PlayerRotationSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../../Components/InputComponents.h"

//TODO: probably move this outside of method, when find a proper way to test this system
inline godot::Vector3 godot::PlayerRotationSystem::GetTargetDirection(Vector2 inputDir, Basis camBasis)
{
	Vector3 dir{ 0, 0, 0 };
	dir += camBasis.x * inputDir.x + camBasis.z * inputDir.y;
	dir.y = 0;

	return dir;
}

//TODO: make smooth rotation
void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<PlayerTag>, RotationDirectionComponent, InputComponent, Camera*>();
	view.less(
	[](RotationDirectionComponent& rotDir, InputComponent input, Camera* pCam)
	{
		if (input.rotation.length_squared() == 0)
			return;

		Basis camBasis = pCam->get_global_transform().get_basis();
		rotDir.direction = GetTargetDirection(input.rotation, camBasis);
	});
}
