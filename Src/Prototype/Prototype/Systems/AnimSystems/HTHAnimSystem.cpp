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
		//TODO0: possibly merge with MeleeAttackSystem and remove copypasted cooldown code
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		bool attackInput = attackComp.prevAnimTimeMillis + utils::SecondsToMillis(attackComp.attackTime) <= currTimeMillis;
		if (attackInput && input.Test(EInput::Attack))
		{
			//TODO0: refactor Combo sequence code after merging with MeleeAttackSystem
			//========Combo sequence code========
			bool continueCombo = attackComp.prevAnimTimeMillis + utils::SecondsToMillis(2.5f) > currTimeMillis;
			if (continueCombo)
			{
				attackComp.comboSequenceNumber++;
				if (attackComp.comboSequenceNumber > 7)//TODO: remove hardcode
					attackComp.comboSequenceNumber = 0;
			}
			else
				attackComp.comboSequenceNumber = 0;
			attackComp.prevAnimTimeMillis = currTimeMillis;
			//===================================

			AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
			String animName(anims[attackComp.comboSequenceNumber]);
			Ref<Animation> anim = pAnimPlayer->get_animation(animName);
			
			String paramName(paramNames[attackComp.comboSequenceNumber]);
			//TODO: same anim time for all animations, maybe use general anim scale for this
			//float timeScale = anim->get_length() / attackComp.attackTime;
			//pAnimTree->set("parameters/" + paramName + "TimeScale/scale", timeScale);
			pAnimTree->set("parameters/" + paramName + "OneShot/active", true);
		}
	});
}