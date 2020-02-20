#include "LocomotionAnimSystem.h"

#include <AnimationTree.hpp>
//#include <AnimationNode.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"
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
		pAnimTree->set("parameters/LocomotionBlendSpace/blend_position", animVec);
	});

	//TODO: this is not locomotion! either move to another system oor remove this system
	auto punchView = registry.view<InputComponent, MeleeAttackComponent, AnimationTree*>();
	punchView.each([](InputComponent input, MeleeAttackComponent attackComp, AnimationTree* pAnimTree)
	{
		if (input.Test(EInput::Attack))
		{
			//TODO0: get AnimationPlayer as component
			AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
			Ref<Animation> anim = pAnimPlayer->get_animation("Jab Left");

			pAnimTree->set("parameters/LeftJabTimeScale/scale", 1 / (anim->get_length() * attackComp.attackTime));
			pAnimTree->set("parameters/LeftJabOneShot/active", true);
		}
	});
}