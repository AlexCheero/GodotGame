#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "entt/entt.hpp"

#include "../Utils/Utils.h"

#include "ComponentsMeta.h"

//used instead of -std::numeric_limits<int64_t>::max() to prevent underflow
constexpr int64_t DEFAULT_PREV_HIT_TIME = -10000;

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
	int64_t prevHitTime = DEFAULT_PREV_HIT_TIME;
	
	//TODO: maybe move outside of the class
	static int64_t maxComboIntervalMillis;

	MeleeHit GetCurrentHit() { return hits[hitIdx]; }
};
REGISTER_COMPONENT(MeleeAttackComponent, "maxPileInDistance");

struct MeleeWeaponComponent
{
	godot::String hitsConfigName;
};
REGISTER_COMPONENT(MeleeWeaponComponent, "hitsConfigName");

DECLARE_TAG(MeleeAttackEvent);
DECLARE_TAG(MeleeAttackBuffered);

DECLARE_TAG(CurrentWeaponRangedTag);
struct RangedAttackComponent
{
	int ammoCount;
	float distance;
	float damage;
	float attackTime;
	int64_t prevHitTime = DEFAULT_PREV_HIT_TIME;
};
REGISTER_COMPONENT(RangedAttackComponent, "ammoCount", "distance", "damage", "attackTime");

DECLARE_TAG(CurrentWeaponThrowableTag);
struct ThrowableAttackComponent
{
	int ammoCount;
	//TODO: implement object pools (for grenades too). keep pooled objects outside of scene tree
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
	int64_t prevHitTime = DEFAULT_PREV_HIT_TIME;
};
REGISTER_COMPONENT(ThrowableAttackComponent, "ammoCount", "throwableScene", "force", "attackTime");

DECLARE_TAG(CurrentWeaponGrenadeTag);
struct GrenadeAttackComponent
{
	int ammoCount;
	godot::Ref<godot::PackedScene> grenadeScene;
	float force;
	float attackTime;
	int64_t prevHitTime = DEFAULT_PREV_HIT_TIME;
};
REGISTER_COMPONENT(GrenadeAttackComponent, "ammoCount", "grenadeScene", "force", "attackTime");

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