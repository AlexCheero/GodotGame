#include "PlayerRotationSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../Components/Player.h"
#include "../Components/InputComponents.h"

//TODO: probably move this outside of method, when find a proper way to test this system
inline godot::Vector3 godot::PlayerRotationSystem::GetTargetDirection(Vector2 inputDir, Basis camBasis)
{
	Vector3 dir{ 0, 0, 0 };
	dir += camBasis.x * inputDir.x + camBasis.z * inputDir.y;
	dir.y = 0;

	return dir;
}

void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<RotationTag>, InputRotationComponent, Player*, Camera*>();
	view.each([&registry](entt::entity entity, entt::tag<RotationTag> rotationComp, InputRotationComponent input, Player* pPlayer, Camera* pCam)
	{
		Basis camBasis = pCam->get_global_transform().get_basis();
		Vector3 dir = GetTargetDirection(input.dir, camBasis);

		if (dir.length_squared() == 0)
			return;
		
		Vector3 lookTarget = pPlayer->get_global_transform().get_origin() - dir;
		pPlayer->look_at(lookTarget, Vector3{ 0, 1, 0 });
	});
}
