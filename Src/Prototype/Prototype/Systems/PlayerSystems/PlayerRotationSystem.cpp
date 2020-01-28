#include "PlayerRotationSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../../Components/InputComponents.h"

//TODO: probably move this outside of method, when find a proper way to test this system
inline godot::Vector3 godot::PlayerRotationSystem::GetTargetDirection(Vector2 inputDir, Basis camBasis)
{
	Vector3 dir{ 0, 0, 0 };
	//TODO1: probably move to utils or make it static. and the same in BillboardRotationSystem
	const Plane xzPlane(Vector3{ 0, 1, 0 }, 0);
	Vector3 relativeLeft = xzPlane.project(camBasis.x).normalized();
	Vector3 relativeFwd = xzPlane.project(camBasis.z).normalized();
	//TODO: check movement and rotation and fix accordingly to the fact that te cam's z is backwards
	dir += relativeLeft * inputDir.x + relativeFwd * inputDir.y;
	dir.y = 0;

	return dir.normalized();
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
