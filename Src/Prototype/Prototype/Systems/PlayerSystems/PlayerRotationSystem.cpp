#include "PlayerRotationSystem.h"

#include <Transform.hpp>
#include <Camera.hpp>
#include <Input.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

#include "../../Utils/Utils.h"

void godot::PlayerRotationSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: MoveDirInputComponent used instead, until aimed view will be implemented
	auto view = registry.view<PlayerTag, RotationDirectionComponent, /*RotationInputComponent*/MoveDirInputComponent, Camera*>();
	view.each([](RotationDirectionComponent& rotDir, /*RotationInputComponent*/MoveDirInputComponent input, Camera* pCam)
	{
		if (input.dir.length_squared() == 0)
			return;

		Basis camBasis = pCam->get_global_transform().get_basis();
		rotDir.direction = utils::GetRelativeFlatDirection(input.dir, camBasis.x, camBasis.z);
	});
}