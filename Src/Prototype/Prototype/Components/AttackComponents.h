#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "entt/entt.hpp"

#include "../Utils/Utils.h"

#include "ComponentsMeta.h"

//TODO: implement component dependance system, to automatically assign on component on assignation dependant to it.
//      e.g. assign PrevAttackTime on assignation of MeleeAttackComponent
struct PrevAttackTime
{
	//TODO: use for other attack types and reset on changing attack type
	int64_t millis = -std::numeric_limits<int64_t>::max();
};

struct MeleeHit
{
	godot::String anim;
	float damage;
	float attackTime;
	float maxDistance;
	float minDistance;
	//TODO: implement
	//std::vector<Effect> effects;
};

DECLARE_TAG(CurrentWeaponMeleeTag);
struct MeleeAttackComponent
{
	float maxPileInDistance;

	std::vector<MeleeHit> hits;

	int hitIdx = 0;
	
	static int64_t maxComboIntervalMillis;

	MeleeHit GetCurrentHit() { return hits[hitIdx]; }
};
REGISTER_COMPONENT(MeleeAttackComponent, "maxPileInDistance");

struct MeleeWeaponComponent
{
	godot::String hitsConfigName;
};
REGISTER_COMPONENT(MeleeWeaponComponent, "hitsConfigName");

DECLARE_TAG(CurrentWeaponRangedTag);
struct RangedAttackComponent
{
	//TODO: move to more hot component and use for throwables too
	int ammoCount;
	float distance;
	float damage;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};
REGISTER_COMPONENT(RangedAttackComponent, "ammoCount", "distance", "damage", "attackTime");

//TODO: make grenade as separate weapon type
DECLARE_TAG(CurrentWeaponThrowableTag);
struct ThrowableAttackComponent
{
	int ammoCount;
	//TODO: implement object pools. keep pooled objects outside of scene tree
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};
REGISTER_COMPONENT(ThrowableAttackComponent, "ammoCount", "throwableScene", "force", "attackTime");

struct AttackAnimPlayingComponent
{
	float playBackTimeLeft;
};

struct TargetLockComponent
{
	entt::entity target = entt::null;
};

DECLARE_TAG(GrenadeExplodesTag);
struct GrenadeComponent
{
	float explosionTime;
	float explosionRadius;
	float damage;
	float startTime;
};
REGISTER_COMPONENT(GrenadeComponent, "explosionTime", "explosionRadius", "damage");

DECLARE_TAG(PileInTag);