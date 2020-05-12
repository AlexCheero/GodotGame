#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "entt/entt.hpp"

#include "../Utils/Utils.h"

#include "ComponentsMeta.h"

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
	std::vector<MeleeHit> hits;

	int hitIdx = 0;
	int64_t prevHitTimeMillis = -utils::SecondsToMillis(hits[0].attackTime);
	
	static int64_t maxComboIntervalMillis;

	MeleeHit GetCurrentHit() { return hits[hitIdx]; }
};

struct MeleeWeaponComponent
{
	godot::String hitsConfigName;
};
REGISTER_COMPONENT(MeleeWeaponComponent, "hitsConfigName");

DECLARE_TAG(CurrentWeaponRangedTag);
struct RangedAttackComponent
{
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
DECLARE_TAG(IncrementComboTag);