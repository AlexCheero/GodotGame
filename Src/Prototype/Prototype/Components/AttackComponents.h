#pragma once

#include <stdint.h>

#include "../Utils.h"

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