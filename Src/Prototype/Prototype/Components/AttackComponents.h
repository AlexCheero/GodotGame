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
	float damage;
	float attackTime;
	godot::String anim;
	float maxDistance;
	float minDistance;
	//TODO: implement
	//std::vector<Effect> effects;
};

DECLARE_REGISTERED_TAG(CurrentWeaponMeleeTag);
struct MeleeAttackComponent
{
	std::vector<MeleeHit> hits;

	//TODO: maybe split into cold and hot data
	int hitIdx = 0;
	int64_t prevHitTimeMillis = -utils::SecondsToMillis(hits[0].attackTime);
	//TODO: remove hardcode
	static constexpr int64_t maxComboIntervalMillis = 3000;

	MeleeHit GetCurrentHit() { return hits[hitIdx]; }
};

struct MeleeWeaponComponent
{
	float damage;
	float attackTime;
	float distance;
};
REGISTER_COMPONENT(MeleeWeaponComponent, "damage", "attackTime", "distance");

DECLARE_REGISTERED_TAG(CurrentWeaponRangedTag);
struct RangedAttackComponent
{
	int ammoCount;
	float distance;
	float damage;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};
REGISTER_COMPONENT(RangedAttackComponent, "ammoCount", "distance", "damage", "attackTime");

//TODO make grenade as separate weapon type
DECLARE_REGISTERED_TAG(CurrentWeaponThrowableTag);
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

DECLARE_TAG(AttackActionTag);
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