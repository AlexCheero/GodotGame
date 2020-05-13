#include "HTHAnimRSystem.h"

#include <AnimationTree.hpp>
#include <Animation.hpp>
#include <AnimationPlayer.hpp>

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

namespace //private
{
	//TODO: try to implement anim system without AnimationTree
	//      and delete EndAttackAnimSystem and AttackAnimPlayingComponent as unnecessary
	void OnInputPressed(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponMeleeTag>(entity))
			return;

		ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");
		ASSERT(registry.has<godot::AnimationTree*>(entity), "entity has no AnimationTree*");

		MeleeAttackComponent attackComp = registry.get<MeleeAttackComponent>(entity);
		godot::AnimationTree* pAnimTree = registry.get<godot::AnimationTree*>(entity);

		int prevAnimIdx = attackComp.hitIdx - 1;
		if (prevAnimIdx < 0)
			prevAnimIdx = attackComp.hits.size() - 1;
		godot::String animName = attackComp.GetCurrentHit().anim;
		godot::String prevAnimName = attackComp.hits[prevAnimIdx].anim;

		godot::AnimationPlayer* pAnimPlayer = godot::Object::cast_to<godot::AnimationPlayer>(pAnimTree->get_node(pAnimTree->get_animation_player()));
		godot::Ref<godot::Animation> anim = pAnimPlayer->get_animation(animName);
		float timeScale = anim->get_length() / attackComp.GetCurrentHit().attackTime;
		pAnimTree->set("parameters/" + animName + "_TimeScale/scale", timeScale);

		registry.assign_or_replace<AttackAnimPlayingComponent>(entity).playBackTimeLeft = anim->get_length() / timeScale;

		//TODO: blend between anims
		pAnimTree->set("parameters/" + prevAnimName + "_OneShot/active", false);
		pAnimTree->set("parameters/" + animName + "_OneShot/active", true);
	}
}

void godot::HTHAnimRSystem::Init(entt::registry& registry)
{
	registry.on_construct<AttackPressedTag>().connect<&OnInputPressed>();
}