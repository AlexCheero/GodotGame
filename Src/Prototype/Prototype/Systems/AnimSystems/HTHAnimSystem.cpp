#include "HTHAnimSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>
#include <OS.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"

//TODO: remove hardcode
static const char* anims[8] =
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

static const char* paramNames[8] =
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
	auto punchView = registry.view<entt::tag<PlayHthAnimTag>, MeleeAttackComponent, AnimationTree*>();
	punchView.less([&registry](entt::entity entity, MeleeAttackComponent& attackComp, AnimationTree* pAnimTree)
	{
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();

		//TODO: remove hardcode
		static const int animCount = 7;

		//TODO0: refactor Combo sequence code after merging with MeleeAttackSystem
		//========Combo sequence code========
		bool continueCombo = attackComp.prevAnimTimeMillis + utils::SecondsToMillis(2.5f) > currTimeMillis;
		if (continueCombo)
		{
			attackComp.comboSequenceNumber++;
			if (attackComp.comboSequenceNumber > animCount)
				attackComp.comboSequenceNumber = 0;
		}
		else
			attackComp.comboSequenceNumber = 0;
		attackComp.prevAnimTimeMillis = currTimeMillis;
		//===================================

		int prevAnimIdx = attackComp.comboSequenceNumber - 1;
		if (prevAnimIdx < 0)
			prevAnimIdx = animCount;
		String prevParamName(paramNames[prevAnimIdx]);
		String paramName(paramNames[attackComp.comboSequenceNumber]);

		//TODO: same anim time for all animations, maybe use general anim scale for this
		//AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
		//String animName(anims[attackComp.comboSequenceNumber]);
		//Ref<Animation> anim = pAnimPlayer->get_animation(animName);
		//float timeScale = anim->get_length() / attackComp.attackTime;
		//pAnimTree->set("parameters/" + paramName + "TimeScale/scale", timeScale);

		//TODO: blend between anims
		pAnimTree->set("parameters/" + prevParamName + "OneShot/active", false);
		pAnimTree->set("parameters/" + paramName + "OneShot/active", true);

		registry.remove<entt::tag<PlayHthAnimTag> >(entity);
	});
}