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

//TODO: move to namespace or something
inline int64_t maxComboIntervalMillis;

struct AttackCooldownComponent
{
	int64_t prevHitTime = DEFAULT_PREV_HIT_TIME;
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

struct MeleePileInComponent
{
	float maxPileInDistance;
};
COMPONENTS_META(MeleePileInComponent, "maxPileInDistance");

TAG(CurrentWeaponMeleeTag);
struct MeleeAttackComponent
{
	std::vector<MeleeHit> hits;
	int hitIdx = 0;
	
	MeleeHit GetCurrentHit() { return hits[hitIdx]; }
};

struct MeleeWeaponComponent
{
	godot::String hitsConfigName;
};
COMPONENTS_META(MeleeWeaponComponent, "hitsConfigName");

TAG(MeleeAttackEvent);
TAG(MeleeAttackBuffered);

TAG(CurrentWeaponRangedTag);
struct RangedAttackComponent
{
	int ammoCount;
	float distance;
	float damage;
	float attackTime;
};
COMPONENTS_META(RangedAttackComponent, "ammoCount", "distance", "damage", "attackTime");

TAG(CurrentWeaponThrowableTag);
struct ThrowableAttackComponent
{
	int ammoCount;
	//TODO: implement object pools (for grenades too). keep pooled objects outside of scene tree
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
};
COMPONENTS_META(ThrowableAttackComponent, "ammoCount", "throwableScene", "force", "attackTime");

TAG(CurrentWeaponGrenadeTag);
struct GrenadeAttackComponent
{
	int ammoCount;
	godot::Ref<godot::PackedScene> grenadeScene;
	float force;
	float attackTime;
};
COMPONENTS_META(GrenadeAttackComponent, "ammoCount", "grenadeScene", "force", "attackTime");

struct AttackAnimPlayingComponent
{
	float playBackTimeLeft;
};

struct TargetLockComponent
{
	entt::entity target = entt::null;
};

struct GrenadeTimerComponent
{
	float startTime;
};

TAG(GrenadeExplodesTag);
struct GrenadeComponent
{
	float explosionTime;
	float explosionRadius;
	float damage;
};
COMPONENTS_META(GrenadeComponent, "explosionTime", "explosionRadius", "damage");

TAG(PileInTag);