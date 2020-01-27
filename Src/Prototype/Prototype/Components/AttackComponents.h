#pragma once

#include <stdint.h>
#include <bitset>

#include <Ref.hpp>
#include <PackedScene.hpp>

#include "../Utils/Utils.h"

struct MeleeAttackComponent
{
	float distance;
	float damage;
	float angle;
	float attackTime;
	godot::String collisionLayerName;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

//TODO: implement ammo count
struct CastAttackComponent
{
	float distance;
	float damage;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

//TODO: implement grenades
struct ThrowableAttackComponent
{
	//TODO: implement object pools
	godot::Ref<godot::PackedScene> throwableScene;
	float force;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};

enum class EWeapons
{
	Melee,
	Ranged,
	Throwable,

	//do not add anything after this value
	End
};

struct WeaponHolderComponent
{
	std::bitset<static_cast<int>(EWeapons::End)> availableWeapons;
	void Set(EWeapons input, bool value) { availableWeapons.set(static_cast<int>(input), value); }
	bool Test(EWeapons input) { return availableWeapons.test(static_cast<int>(input)); }

	MeleeAttackComponent melee;
	CastAttackComponent ranged;
	ThrowableAttackComponent throwable;
};