#include "HTHAnimSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>

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
	auto punchView = registry.view<AttackActionTag, MeleeAttackComponent, AnimationTree*>();
	punchView.less([&registry](entt::entity entity, MeleeAttackComponent& attackComp, AnimationTree* pAnimTree)
	{
		int prevAnimIdx = attackComp.comboSequenceNum - 1;
		if (prevAnimIdx < 0)
			prevAnimIdx = attackComp.comboLength - 1;
		String prevParamName(paramNames[prevAnimIdx]);
		String paramName(paramNames[attackComp.comboSequenceNum]);

		//TODO: same anim time for all animations, maybe use general anim scale for this
		AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
		String animName(anims[attackComp.comboSequenceNum]);
		Ref<Animation> anim = pAnimPlayer->get_animation(animName);
		//float timeScale = anim->get_length() / attackComp.attackTime;
		//pAnimTree->set("parameters/" + paramName + "TimeScale/scale", timeScale);

		//TODO: take anim time scale into account
		registry.assign_or_replace<AttackAnimPlayingComponent>(entity).playBackTimeLeft = anim->get_length();

		//TODO: blend between anims
		pAnimTree->set("parameters/" + prevParamName + "OneShot/active", false);
		pAnimTree->set("parameters/" + paramName + "OneShot/active", true);
	});
}