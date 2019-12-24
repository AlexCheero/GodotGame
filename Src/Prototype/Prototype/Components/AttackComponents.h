#pragma once

#include <stdint.h>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "../Utils.h"

//TODO: make base attack component
struct MelleAttackComponent
{
	float distance;
	float damage;
	float angle;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

struct CastAttackComponent
{
	float distance;
	float damage;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

struct ThrowableAttackComponent
{
	//TODO: implement object pools
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

struct WeaponHolderComponent
{
	MelleAttackComponent melee;
	CastAttackComponent ranged;
	ThrowableAttackComponent throwable;
};