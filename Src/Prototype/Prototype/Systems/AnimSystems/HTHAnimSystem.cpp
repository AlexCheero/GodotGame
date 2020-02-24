#include "HTHAnimSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>
#include <OS.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"

const char* anims[8] =
{
	"Jab Left",
	"Jab Right",
	"Hook Left",
	"Hook Right",
	"Kick Front Left",
	"Kick Front Right",
	"Kick Round Left",
	"Kick Round Right"
};

const char* paramNames[8] =
{
	"LeftJab",
	"RightJab",
	"LeftHook",
	"RightHook",
	"FrontLeftKick",
	"FrontRightKick",
	"RoundLeftKick",
	"RoundRightKick"
};

void godot::HTHAnimSystem::operator()(float delta, entt::registry& registry)
{
	auto punchView = registry.view<InputComponent, MeleeAttackComponent, AnimationTree*>();
	punchView.each([](entt::entity entity, InputComponent& input, MeleeAttackComponent& attackComp, AnimationTree* pAnimTree)
	{
		//TODO0: possibly merge with MeleeAttackSystem and use Test instead of TestAndReset
		if (input.TestAndReset(EInput::Attack))
		{
			AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
			Ref<Animation> anim = pAnimPlayer->get_animation(anims[attackComp.comboSequenceNumber]);
			
			String paramName = String(paramNames[attackComp.comboSequenceNumber]);
			pAnimTree->set("parameters/" + paramName + "TimeScale/scale",
				1 / (anim->get_length() * attackComp.attackTime));
			pAnimTree->set("parameters/" + paramName + "OneShot/active", true);
			
			attackComp.comboSequenceNumber++;
			if (attackComp.comboSequenceNumber > 7)
				attackComp.comboSequenceNumber = 0;
		}
	});
}