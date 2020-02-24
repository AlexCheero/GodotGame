#include "HTHAnimSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"

void godot::HTHAnimSystem::operator()(float delta, entt::registry& registry)
{
	auto punchView = registry.view<InputComponent, MeleeAttackComponent, AnimationTree*>();
	punchView.each([](InputComponent input, MeleeAttackComponent attackComp, AnimationTree* pAnimTree)
	{
		if (input.Test(EInput::Attack))
		{
			AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
			Ref<Animation> anim = pAnimPlayer->get_animation("Jab Left");

			pAnimTree->set("parameters/LeftJabTimeScale/scale", 1 / (anim->get_length() * attackComp.attackTime));
			pAnimTree->set("parameters/LeftJabOneShot/active", true);
		}
	});
}