#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "entt/entt.hpp"

#include "../Utils/Utils.h"

constexpr entt::hashed_string CurrentWeaponMeleeTag = "CurrentWeaponMeleeTag"_hs;
struct MeleeAttackComponent
{
	float distance;
	float angle;
	//TODO: set attack time in anim
	float attackTime;
	godot::String collisionLayerName;
	int64_t prevHitTimeMillis = -utils::SecondsToMillis(attackTime);
	//TODO0: hack!!! remove when HTHAnimSystem merged with MeleeAttackSystem
	int64_t prevAnimTimeMillis = -utils::SecondsToMillis(attackTime);
	entt::entity lockedTarget = entt::null;
	int comboSequenceNumber = 0;
};

constexpr entt::hashed_string CurrentWeaponRangedTag = "CurrentWeaponRangedTag"_hs;
struct RangedAttackComponent
{
	int ammoCount;
	float distance;
	float damage;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

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

constexpr entt::hashed_string GrenadeExplodesTag = "GrenadeExplodesTag"_hs;
struct GrenadeComponent
{
	float explosionTime;
	float explosionRadius;
	float damage;
	float startTime;
};