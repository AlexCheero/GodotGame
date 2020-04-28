#include "HTHAnimSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"

//TODO: remove hardcode
static const char* anims[8] =
{
	"Jab_Left",
	"Jab_Right",
	"Hook_Left",
	"Hook_Right",
	"Kick_Front_Left",
	"Kick_Front_Right",
	"Kick_Round_Left",
	"Kick_Round_Right"
};

//TODO: don't interrupt previous hit by the next one
void godot::HTHAnimSystem::operator()(float delta, entt::registry& registry)
{
	auto punchView = registry.view<AttackActionTag, MeleeAttackComponent, AnimationTree*>();
	punchView.less([&registry](entt::entity entity, MeleeAttackComponent& attackComp, AnimationTree* pAnimTree)
	{
		int prevAnimIdx = attackComp.comboSequenceNum - 1;
		if (prevAnimIdx < 0)
			prevAnimIdx = attackComp.comboLength - 1;
		String animName(anims[attackComp.comboSequenceNum]);
		String prevAnimName(anims[prevAnimIdx]);

		//TODO: same anim time for all animations, maybe use general anim scale for this
		AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
		Ref<Animation> anim = pAnimPlayer->get_animation(animName);
		//float timeScale = anim->get_length() / attackComp.attackTime;
		//pAnimTree->set("parameters/" + paramName + "_TimeScale/scale", timeScale);

		//TODO: take anim time scale into account
		registry.assign_or_replace<AttackAnimPlayingComponent>(entity).playBackTimeLeft = anim->get_length();

		//TODO: blend between anims
		pAnimTree->set("parameters/" + prevAnimName + "_OneShot/active", false);
		pAnimTree->set("parameters/" + animName + "_OneShot/active", true);
	});
}