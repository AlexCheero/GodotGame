#include "MeleeAnimSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>

#include "../../../Components/InputComponents.h"
#include "../../../Components/AttackComponents.h"

//TODO: try to use AnimationNodeStateMachine, get is_playing via AnimationNodeStateMachinePlayback
//      and delete EndAttackAnimSystem and AttackAnimPlayingComponent as unnecessary
void godot::MeleeAnimSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent, AnimationTree*>();
	view.less([&registry](entt::entity entity, MeleeAttackComponent attackComp, AnimationTree* pAnimTree)
	{
		int prevAnimIdx = attackComp.hitIdx - 1;
		if (prevAnimIdx < 0)
			prevAnimIdx = attackComp.hits.size() - 1;
		String animName = attackComp.GetCurrentHit().anim;
		String prevAnimName = attackComp.hits[prevAnimIdx].anim;

		AnimationPlayer* pAnimPlayer = Object::cast_to<AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
		Ref<Animation> anim = pAnimPlayer->get_animation(animName);
		float timeScale = anim->get_length() / attackComp.GetCurrentHit().attackTime;
		pAnimTree->set("parameters/" + animName + "_TimeScale/scale", timeScale);

		registry.assign_or_replace<AttackAnimPlayingComponent>(entity).playBackTimeLeft = anim->get_length() / timeScale;

		//TODO: blend between anims
		pAnimTree->set("parameters/" + prevAnimName + "_OneShot/active", false);
		pAnimTree->set("parameters/" + animName + "_OneShot/active", true);
	});
}