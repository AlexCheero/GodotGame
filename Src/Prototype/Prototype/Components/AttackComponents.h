#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "entt/entt.hpp"

#include "../Utils/Utils.h"

#include "ComponentsMeta.h"

constexpr entt::hashed_string CurrentWeaponMeleeTag = "CurrentWeaponMeleeTag"_hs;
//TODO: refactor this component. split into several separate ones
struct MeleeAttackComponent
{
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
REGISTER_COMPONENT(MeleeAttackComponent, "attackTime", "collisionLayerName", "maxComboIntervalMillis", "stunTime", "stunSpeedFactor", "maxDistance", "minDistance", "dashSpeed");

constexpr entt::hashed_string CurrentWeaponRangedTag = "CurrentWeaponRangedTag"_hs;
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
constexpr entt::hashed_string CurrentWeaponThrowableTag = "CurrentWeaponThrowableTag"_hs;
struct ThrowableAttackComponent
{
	int ammoCount;
	//TODO: implement object pools
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};
REGISTER_COMPONENT(ThrowableAttackComponent, "ammoCount", "throwableScene", "force", "attackTime");

constexpr entt::hashed_string AttackActionTag = "AttackActionTag"_hs;
struct AttackAnimPlayingComponent
{
	float playBackTimeLeft;
};

struct TargetLockComponent
{
	entt::entity target = entt::null;
};

constexpr entt::hashed_string GrenadeExplodesTag = "GrenadeExplodesTag"_hs;
struct GrenadeComponent
{
	float explosionTime;
	float explosionRadius;
	float damage;
	float startTime;
};
REGISTER_COMPONENT(GrenadeComponent, "explosionTime", "explosionRadius", "damage");

constexpr entt::hashed_string PileInTag = "PileInTag"_hs;
constexpr entt::hashed_string IncrementComboTag = "IncrementComboTag"_hs;