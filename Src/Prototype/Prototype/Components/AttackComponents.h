#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "entt/entt.hpp"

#include "../Utils/Utils.h"

#include "ComponentsMeta.h"

DECLARE_TAG(CurrentWeaponMeleeTag);
//TODO: refactor this component. split into several separate ones
struct MeleeAttackComponent
{
	float damage;
	//TODO: set attack time in anim
	float attackTime;
	godot::String collisionLayerName;
	int64_t maxComboIntervalMillis;
	//TODO: probably move this to ability component (or something like this)
	float stunTime;
	float stunSpeedFactor;

	//<pile in
	float maxDistance;
	float minDistance;
	float dashSpeed;
	//pile in>

	//TODO: remove hardcode
	int comboLength = 8;
	
	//TODO: maybe split into cold and hot data
	int64_t prevHitTimeMillis = -utils::SecondsToMillis(attackTime);
	int comboSequenceNum = 0;
};
REGISTER_COMPONENT(MeleeAttackComponent, "damage", "attackTime", "collisionLayerName", "maxComboIntervalMillis",
	                                     "stunTime", "stunSpeedFactor", "maxDistance", "minDistance", "dashSpeed");

struct MeleeWeaponComponent
{
	float damage;
	float attackTime;
	float distance;
};
REGISTER_COMPONENT(MeleeWeaponComponent, "damage", "attackTime", "distance");

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

//TODO make grenade as separate weapon type
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