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
	//TODO: move this declaration into macro
	constexpr static int PROPERTIES_COUNT = 4;
	//TODO: use different distance for each hit or it should be defined by animation
	float distance;
	float angle;
	//TODO: set attack time in anim
	float attackTime;
	godot::String collisionLayerName;
	//TODO: maybe split into cold and hot data
	int64_t prevHitTimeMillis = -utils::SecondsToMillis(attackTime);
	//TODO: remove hardcode and get from view
	int64_t maxComboIntervalMillis = 3000;
	int comboLength = 8;
	int comboSequenceNum = 0;
};

constexpr entt::hashed_string CurrentWeaponRangedTag = "CurrentWeaponRangedTag"_hs;
struct RangedAttackComponent
{
	constexpr static int PROPERTIES_COUNT = 4;
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
	constexpr static int PROPERTIES_COUNT = 4;
	int ammoCount;
	//TODO: implement object pools
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

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
	constexpr static int PROPERTIES_COUNT = 4;
	float explosionTime;
	float explosionRadius;
	float damage;
	float startTime;
};

constexpr entt::hashed_string PileInTag = "PileInTag"_hs;

struct HTHStuckComponent
{
	float secondsLeft;
};

constexpr entt::hashed_string IncrementComboTag = "IncrementComboTag"_hs;