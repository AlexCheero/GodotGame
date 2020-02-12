#include "LocomotionAnimSystem.h"

#include <AnimationTree.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Utils/Utils.h"

void godot::LocomotionAnimSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, RotationDirectionComponent, AnimationTree*>();
	view.each([](VelocityComponent velComp, RotationDirectionComponent rotComp, AnimationTree* pAnimTree)
	{
		Vector3 bZ = rotComp.direction;
		Vector3 bY = utils::globalY;
		Vector3 bX = bZ.cross(bY).normalized();

		Basis b(bX, bY, bZ);
		b.orthonormalize();
		Vector3 flatVel = velComp.velocity;
		flatVel.y = 0;
		flatVel.normalize();
		Vector3 xform = b.xform(flatVel).normalized();

		Vector2 animVec{ xform.x, xform.z };

		//TODO: diagonal anims looks a bit weird because of blending problem
		pAnimTree->set("parameters/BlendSpace2D/blend_position", animVec);
	});

	auto punchView = registry.view<InputComponent, AnimationTree*>();
	punchView.each([](InputComponent input, AnimationTree* pAnimTree)
	{
		if (input.Test(EInput::Attack))
			pAnimTree->set("parameters/OneShot/active", true);
	});
}